#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

int main() {
  std::error_code ec;
  std::string path1 =
      "/tmp/adkdkdkdkkdkdkdkasdadfadfadfasdffadsfadsfasdfasdfasdf";
  std::string path2 = "/tmp/dkdkasd323asdkfasdifi2ef";
  fs::rename(path1, path2, ec);
  if (ec) {
    std::cout << "test ec: " << ec.message() << std::endl;
  } else {
    std::cout << "test ok" << std::endl;
  }
  return 0;
}
