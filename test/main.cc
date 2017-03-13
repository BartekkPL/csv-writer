#include "gtest/gtest.h"

#include "config.h"
#include "csv-writer.h"
#include "easylogging++.h"

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace bf = boost::filesystem;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  // Load configuration from file
  el::Configurations conf(EASYLOGGING_CONFIG_FILE_PATH);
  // Reconfigure single logger
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ExampleFileTest, WriteTest) {
  std::vector<std::string> labels;
  labels.push_back("Firstname");
  labels.push_back("Lastname");
  labels.push_back("Age");

  std::vector<std::string> data;
  data.push_back("John");
  data.push_back("Doe");
  data.push_back("23");
  data.push_back("Angelina");
  data.push_back("Smith");
  data.push_back("24");

  ASSERT_TRUE(csv::Csv_writer::write_to_csv<std::string>(labels, data,
                                                         "test.csv"));

  std::vector<std::string> read_labels;
  std::vector<std::string> read_data;
  ASSERT_TRUE(csv::Csv_writer::read_from_csv<std::string>(read_labels,
      read_data, "test.csv"));
  ASSERT_EQ(labels, read_labels);
  ASSERT_EQ(data, read_data);
}