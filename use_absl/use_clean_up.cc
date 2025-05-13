#include <absl/cleanup/cleanup.h>

#include <fstream>
#include <iostream>

void processFile(const std::string& filename) {
  // Open the file
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // Create a cleanup action that will close the file when the scope is exited
  absl::Cleanup cleanup = [&file]() {
    if (file.is_open()) {
      file.close();
      std::cout << "File closed by cleanup." << std::endl;
    }
  };

  // Write some data to the file
  file << "Hello, World!" << std::endl;

  // The file will be automatically closed when the function scope is exited
  // either by reaching the end, returning, or throwing an exception.
}

void multi_return(int x) {
  absl::Cleanup cleanup = [&x]() {
    std::cout << "cleanup triggered. x: " << x << std::endl;
  };

  if (x <= 0) {
    std::cout << "less than 1\n";
  }
  if (x == 1) {
    std::cout << " == 1\n";
    return;
  }
  std::cout << " end of " << __func__ << std::endl;
}

int main(int argc, char** argv) {
  processFile("example.txt");
  multi_return(argc);
  return 0;
}
