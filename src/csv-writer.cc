#include "csv-writer.h"

#include "easylogging++.h"

namespace csv {

template <typename T>
static bool Csv_writer::write_to_csv(const std::vector<std::string> labels,
                                      const std::vector<T> data,
                                      const boost::filesystem::path file_path) {
  if (bf::exists(file_path))
    LOG(WARNING) << "File with that name already exists! Overwriting...!";

  std::ofstream file;
  file.open(file_path.string(), std::ofstream::trunc);

  if (!file.is_open()) {
    LOG(ERROR) << "Cannot open the file: " << file_path;
    return false;
  }

  unsigned row_num = labels.size();

  for (int i = 0; i < row_num - 1; i++) {
    file << labels[i] << ",";
  }
  file << labels[row_num - 1];

  for (int i = 0; i < data.size(); i++) {
    if (i % row_num == 0) {
      file << std::endl;
    }
    else {
      file << ",";
    }
    file << data[i];
  }

  return true;
}

template <typename T>
static bool Csv_writer::write_to_csv(const std::vector<std::string> labels,
                                      const std::vector<std::vector<T>> data,
                                      const boost::filesystem::path file_path) {
  if (labels.size() == 0 || data.size() == 0) {
    LOG(ERROR) << "You passed empty vector!";
    return false;
  }

  if (labels.size() != data.size()) {
    LOG(ERROR) << "The number of labels and data vectors should be equal!";
    return false;
  }

  for (int i = 1; i < data.size(); i++) {
    if (data[i].size() != data[0].size()) {
      LOG(ERROR) << "The number of elements of each data vector should be "
                  << "equal!";
      return false;
    }
  }

  // data shuffle
  std::vector<T> data_shuffled;
  for (int i = 0; i < data[0].size(); i++)
    for (int j = 0; j < data.size(); j++)
      data_shuffled.push_back(data[j][i]);

  return write_to_csv(labels, data_shuffled, file_path);
}

template <typename T>
static bool Csv_writer::read_from_csv(std::vector<std::string>& labels,
                                      std::vector<T>& data,
                                      const boost::filesystem::path file_path) {
  if (!bf::exists(file_path)) {
    LOG(ERROR) << "File with that name not exists! Cannot read!";
    return false;
  }

  std::ifstream file;
  file.open(file_path.string());

  if (!file.is_open()) {
    LOG(ERROR) << "Cannot open the file: " << file_path;
    return false;
  }

  std::string token;
  std::string labels_line;
  std::getline(file, labels_line);

  size_t pos = 0;
  while ((pos = labels_line.find(",")) != std::string::npos) {
    token = labels_line.substr(0, pos);
    labels.push_back(token);
    labels_line.erase(0, pos + 1);
  }
  labels.push_back(labels_line);

  while (std::getline(file, token, ',')) {
    std::istringstream iss(token);
    T data_part;
    // when new line there is possible two data between next commas
    while (iss.rdbuf()->in_avail() != 0) {
      iss >> data_part;
      data.push_back(data_part);
    }
  }

  return true;
}

} // namespace