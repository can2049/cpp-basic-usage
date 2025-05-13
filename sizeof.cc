#include <iostream>
#include <type_traits>
#include <vector>
int main() {
  std::cout << "Size of char: " << sizeof(char) << " byte" << std::endl;
  std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
  std::cout << "Size of float: " << sizeof(float) << " bytes" << std::endl;
  std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;
  std::cout << "Size of bool: " << sizeof(bool) << " bytes" << std::endl;
  std::cout << "Size of long: " << sizeof(long) << " bytes" << std::endl;
  std::cout << "Size of long long: " << sizeof(long long) << " bytes"
            << std::endl;

  return 0;
}
