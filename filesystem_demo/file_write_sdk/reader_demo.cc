
#include <sys/types.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>

#include "FileNotifier.hpp"

void handleFileEvent(const std::string& file_path) {
  // check if file exists
  if (!std::filesystem::exists(file_path)) {
    std::cerr << "File does not exist. path: " << file_path << std::endl;
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

  uint32_t mask =
      IN_CLOSE_WRITE | IN_MODIFY | IN_MOVED_TO | IN_MODIFY | IN_ATTRIB;

  // auto mask = IN_MODIFY | IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE |
  //             IN_CLOSE_NOWRITE | IN_OPEN | IN_DELETE_SELF | IN_MOVE_SELF;

  try {
    FileNotifier notifier(file_path, mask, handleFileEvent);
    notifier.start();
    std::cout << "Monitoring started. Press 'Esc' to exit..." << std::endl;
    while (true) {
      auto c = std::cin.get();
      if (c == 27) {  // ASCII code for 'Esc'
        std::cout << "Exiting..." << std::endl;
        break;
      }
    }
    notifier.stop();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
