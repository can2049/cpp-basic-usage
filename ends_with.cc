#include <iostream>
#include <string>
using namespace std;

bool endsWith(const std::string& str, const std::string& suffix) {
  auto suffixLen = suffix.length();
  return str.length() >= suffixLen &&
         0 == str.compare(str.length() - suffixLen, suffixLen, suffix);
}

int main() {
  auto input = "/sensors/camera/camera_1_raw_data/compressed";
  std::string suffix = "/compressed";
  cout << endsWith(input, suffix) << endl;
}