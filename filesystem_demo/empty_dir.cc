#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
int main() {
  std::string path;

  std::cout << "directory? " << fs::is_directory(path) << std::endl;

  std::cout << "empty? " << fs::is_empty("/tmp") << std::endl;

  return 0;
}
