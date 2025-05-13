#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

int main() {
  std::vector<std::string> paths = {
      "/tmp/abcdeftdskaskfkasdfkasdkfkasdkfaksdfkasdfa",
      "/tmp/a/b/c/d/e/f/g/h/i/j/k/l/m/n/o/p/q/r/s/t/u/v/w/x/y/z",
      "/abcdefg",
      "/",
      "/root",
      "/opt",
      "/proc/"};
  for (const auto& path : paths) {
    std::error_code ec;
    auto remove_count = std::filesystem::remove_all(path, ec);
    if (ec) {
      std::cerr << " try to remove path: " << path
                << " . error: " << ec.message() << std::endl;
    } else {
      std::cout << " try to remove path: " << path
                << " success. remove count: " << remove_count << std::endl;
    }
  }
  std::cout << " remove_all done" << std::endl;
  return 0;
}
