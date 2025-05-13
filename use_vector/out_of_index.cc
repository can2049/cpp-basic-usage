#include <array>
#include <iostream>
#include <vector>

int main() {
  std::vector data = {1, 2, 3};
  std::cout << " data size: " << std::size(data) << std::endl;
  auto d = data[10];
  std::cout << " d: " << d << std::endl;
  return 0;
}
