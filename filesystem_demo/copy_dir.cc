#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
  std::string d1 = "/tmp/test-dir-delete-me-later-1";
  std::string d2 = "/tmp/test-dir-delete-me-later-2";

  fs::remove_all(d1);
  fs::remove_all(d2);

  fs::create_directories(d1 + "/dir1");
  fs::create_directory(d2);

  // write some files in d1
  std::ofstream(d1 + "/file1.txt") << "Hello, World!";

  fs::copy(d1, d2,
           fs::copy_options::recursive | fs::copy_options::overwrite_existing);

  std::cout << "Directory copied successfully." << std::endl;

  // print all items in d1
  std::cout << "Contents of " << d1 << ":" << std::endl;
  for (const auto& entry : fs::directory_iterator(d1)) {
    std::cout << entry.path() << std::endl;
  }
  // print all items in d2
  std::cout << "Contents of " << d2 << ":" << std::endl;
  for (const auto& entry : fs::directory_iterator(d2)) {
    std::cout << entry.path() << std::endl;
  }

  return 0;
}