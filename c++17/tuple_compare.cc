#include <iostream>
#include <string>
#include <tuple>

int main() {
  std::tuple<int, int, int> a(1, 2, 4);
  std::tuple<int, int, int> b(1, 3, 3);
  std::tuple<int, int, float> c(2, 2, 3);
  //   std::tuple<int, int, std::string> d(2, 2, "3");
  std::cout << (a < b) << std::endl;
  std::cout << (a < c) << std::endl;
  //   std::cout << (a < d) << std::endl;
}
