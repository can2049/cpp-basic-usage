#include <array>
#include <iostream>

int main() {
  using bitmap_16_bit = std::array<bool, 16>;
  bitmap_16_bit data = {true};
  bitmap_16_bit data2 = {false};
  if (data != bitmap_16_bit{false}) {
    std::cout << "1 has value" << std::endl;
  }
  if (data2 != bitmap_16_bit{false}) {
    std::cout << "2 has value" << std::endl;
  }
}
