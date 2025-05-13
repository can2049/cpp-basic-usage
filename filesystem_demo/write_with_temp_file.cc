#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

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

int main() {
  std::string file_name = "/tmp/test-file-delete-me.txt";
  std::string temp_file = TempFile(file_name, 6);
  std::cout << " get temp_file: " << temp_file << std::endl;

  std::error_code ec;
  //   fs::remove(file_name, ec);
  std::cout << " file exists? " << fs::exists(file_name, ec) << std::endl;

  // 创建一个临时文件
  std::ofstream ofs(temp_file);
  if (!ofs) {
    std::cerr << "Failed to create temporary file: " << temp_file << std::endl;
    return 1;
  }

  // 写入数据
  ofs << "Hello, World!" << std::endl;
  ofs.close();

  // 读取数据
  std::ifstream ifs(temp_file);
  if (!ifs) {
    std::cerr << "Failed to open temporary file: " << temp_file << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(ifs, line)) {
    std::cout << line << std::endl;
  }

  ifs.close();

  // 删除临时文件
  fs::rename(temp_file, file_name, ec);
  if (ec) {
    std::cerr << "Failed to rename temporary file: " << ec.message()
              << std::endl;
    return 1;
  }

  // rename same path
  std::cout << "rename same path: " << file_name << std::endl;
  fs::rename(file_name, file_name);

  std::cout << " temporary file renamed to: " << file_name << std::endl;

  // check if files exist
  std::cout << " temp file: " << temp_file << " exist? "
            << fs::exists(temp_file, ec) << std::endl;
  std::cout << " file: " << file_name << " exist? " << fs::exists(file_name, ec)
            << std::endl;

  for (int i = 0; i < 10; i++) {
    // generate a random file name
    std::string random_file = TempFile("/tmp/test.txt", 6);
    std::cout << "Random file name " << i << ": " << random_file << std::endl;
  }

  return 0;
}
