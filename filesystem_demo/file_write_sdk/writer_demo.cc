
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

#include "SafeFileWriter.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
    return 1;
  }

  std::string file_path = argv[1];
  std::cout << "Monitoring path: " << file_path << std::endl;

  // create parent directory
  std::string file_dir = file_path.substr(0, file_path.find_last_of('/'));
  std::cout << "Creating parent directory: " << file_dir << std::endl;

  std::error_code ec;
  std::filesystem::create_directories(file_dir, ec);
  if (ec) {
    std::cerr << "Error creating directories. path: " << file_dir
              << " message: " << ec.message() << std::endl;
    return 1;
  }

  std::string content = "Hello, World! File writer demo. ";
  auto current_time = std::time(nullptr);
  content += "Current time: " + std::string(std::ctime(&current_time)) + "\n";
  std::cout << "Content to write: " << content << std::endl;

  std::cout << "write path: " << file_dir << std::endl;

  std::filesystem::create_directories(file_dir, ec);
  if (ec) {
    std::cerr << "Error creating directories. path: " << file_dir
              << " message: " << ec.message() << std::endl;
    return 1;
  }

  std::cout << "Creating file: " << file_path << std::endl;
  if (SafeFileWriter::write(file_path, content)) {
    std::cout << "Write successful. path: " << file_path << std::endl;
  } else {
    std::cerr << "Write failed. path: " << file_path << std::endl;
  }
  return 0;
}
