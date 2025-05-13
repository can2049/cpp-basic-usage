#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

double hex2double(uint64_t bits) {
  // Reinterpret the binary data as a double
  return *reinterpret_cast<double*>(&bits);
}

uint64_t double2hex(double num) {
  // Reinterpret the binary data as a double
  return *reinterpret_cast<uint64_t*>(&num);
}

int main() {
  double num = 3.14;

  auto result2 = double2hex(num);
  auto result1 = hex2double(result2);

  std::cout << std::hex << num << " in hex is: " << result2 << "\n";
  std::cout << std::hex << result2 << " in double is: " << result1 << "\n";

  return 0;
}
