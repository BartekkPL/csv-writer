#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "easylogging++.h"

#include <boost/filesystem.hpp>
#include <vector>

namespace bf = boost::filesystem;

namespace csv {

class Csv_writer {
 public:
  template <typename T>
  static bool write_to_csv(const std::vector<std::string> labels,
                           const std::vector<T> data,
                           const boost::filesystem::path file_path) {
    if (bf::exists(file_path)) {
      LOG(ERROR) << "File with that name exists! Cannot write!";
      return false;
    }

    std::ofstream file;
    file.open(file_path.string());

    if (!file.is_open()) {
      LOG(ERROR) << "Cannot create and open the file: " << file_path;
      return false;
    }

    unsigned col_num = labels.size();

    for (int i = 0; i < col_num - 1; i++) {
      file << labels[i] << ",";
    }
    file << labels[col_num - 1];

    for (int i = 0; i < data.size(); i++) {
      if (i % col_num == 0) {
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
  static bool read_from_csv(std::vector<std::string>& labels,
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

};

} // namespace
#endif  // CSV_WRITER_H