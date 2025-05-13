#include <iostream>
#include <string>
using namespace std;
int main() {
  string image_file_name = "aaa.png";

  auto format_pos = image_file_name.rfind(".png");
  if (format_pos != std::string::npos) {
    // image_file_name.replace
  }
}