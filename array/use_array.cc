#include <array>
#include <iostream>

int main() {
  std::array<bool, 16> data = {true};
  std::cout << data.size() << std::endl;
  for (size_t i = 0; i < data.size(); i++) {
    std::cout << data[i] << ", ";
  }
  std::cout << std::endl;
}
