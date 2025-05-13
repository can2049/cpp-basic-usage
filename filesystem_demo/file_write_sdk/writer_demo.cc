
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

#include "SafeFileWriter.hpp"

int main() {
  std::string content = "Hello, World!\nFile writer demo.\n";
  std::string file_dir = "/tmp/file_write_sdk_demo_delete";

  std::error_code ec;
  std::filesystem::create_directories(file_dir, ec);
  if (ec) {
    std::cerr << "Error creating directories. path: " << file_dir
              << " message: " << ec.message() << std::endl;
    return 1;
  }

  std::string file_name = "test.txt";
  std::string file_path = file_dir + "/" + file_name;

  if (SafeFileWriter::write(file_path, content)) {
    std::cout << "Write successful. path: " << file_path << std::endl;
  } else {
    std::cerr << "Write failed. path: " << file_path << std::endl;
  }
  return 0;
}
