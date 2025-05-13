#include "SafeFileWriter.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <system_error>

std::string TempFile(const std::string& file_name, size_t length) {
  constexpr char kChars[] =
      "0123456789"
      "abcdefghijklmnopqrstuvwxyz";

  // Mersenne Twister random number generator
  // https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
  static std::mt19937 generator(std::random_device{}());

  // size - 2 , because we need to skip the last '\0' character
  std::uniform_int_distribution<> dist(0, std::size(kChars) - 2);

  std::string tmp_str;
  tmp_str.reserve(length);  // reserve space for performance

  for (size_t i = 0; i < length; ++i) {
    tmp_str += kChars[dist(generator)];
  }

  std::string temp_file = file_name + "." + tmp_str + ".tmp";
  return temp_file;
}

bool SafeFileWriter::write(const std::string& file_path,
                           const std::string& content) {
  std::string temp_path = TempFile(file_path, 6);

  // 写入临时文件
  std::ofstream file(temp_path, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening temp file. path: " << temp_path << std::endl;
    return false;
  }
  file.write(content.data(), content.size());
  file.close();
  if (!file.good()) {
    std::cerr << "Error writing temp file. path: " << temp_path << std::endl;
    return false;
  }

  // use std::filesystem::rename
  std::error_code ec;
  std::filesystem::rename(temp_path, file_path, ec);
  if (ec) {
    std::cerr << "Error renaming file. path: " << file_path
              << " message: " << ec.message() << std::endl;
    std::filesystem::remove(temp_path, ec);
    return false;
  }

  return true;
}
