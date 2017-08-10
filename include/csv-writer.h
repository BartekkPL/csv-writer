#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "easylogging++.h"

#include <boost/filesystem.hpp>
#include <vector>

namespace bf = boost::filesystem;

namespace csv {

class Csv_writer {
 public:
  /** Write to CSV file method
    *
    * Main csv-writer method which simply write vector of simple type data to
    * new .csv file
    * Template type should be basic type e.g. int, char, string... or class with
    * '<<' operator overloaded
    *
    * @param labels     headers for csv data
    * @param data
    * @param file_path  path to the file
    * @return           true if success, false otherwise
    */
  template <typename T>
  static bool write_to_csv(const std::vector<std::string> labels,
                            const std::vector<T> data,
                            const boost::filesystem::path file_path);

  /** Write to CSV file method
    *
    * Main csv-writer method which simply write vector of vector of simple type
    * data to new .csv file.
    * Template type should be basic type e.g. int, char, string... or class with
    * '<<' operator overloaded.
    * E.g. vector of value of type T that is is first in vector is written as a
    * column in csv with first label from labels vector
    *
    * @param labels     headers for csv data
    * @param data
    * @param file_path  path to the file
    * @return           true if success, false otherwise
    */
  template <typename T>
  static bool write_to_csv(const std::vector<std::string> labels,
                            const std::vector<std::vector<T>> data,
                            const boost::filesystem::path file_path);

  /** Read from CSV file method
    *
    * Additional csv-writer method which simply read vector of simple type data
    * to new .csv file
    * Template type should be basic type e.g. int, char, string... or class with
    * '>>' operator overloaded
    *
    * @param labels     headers from csv data
    * @param data       data read from file
    * @param file_path  path to the file
    * @return           true if success, false otherwise
    */
  template <typename T>
  static bool read_from_csv(std::vector<std::string> &labels,
                            std::vector<T> &data,
                            const boost::filesystem::path file_path);

};

} // namespace
#endif  // CSV_WRITER_H