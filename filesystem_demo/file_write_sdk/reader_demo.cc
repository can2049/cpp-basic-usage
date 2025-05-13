
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>

#include "FileNotifier.hpp"

void handleFileEvent(const std::string& file_path) {
  if (std::filesystem::path(file_path).extension() == ".tmp") {
    std::cerr << "Found a temp file. path: " << file_path << std::endl;
    return;
  }
  std::ifstream file(file_path, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open path: " << file_path << std::endl;
    return;
  }
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  std::cout << "New file: " << file_path << " content: " << content
            << std::endl;
}

int main() {
  std::string file_dir = "/tmp/file_write_sdk_demo_delete";
  std::error_code ec;
  std::filesystem::create_directories(file_dir, ec);
  if (ec) {
    std::cerr << "Error creating directories. path: " << file_dir
              << " message: " << ec.message() << std::endl;
    return 1;
  }

  try {
    FileNotifier notifier(file_dir, handleFileEvent);
    notifier.start();
    std::cout << "Monitoring started. Press Enter to exit..." << std::endl;
    std::cin.get();
    notifier.stop();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
