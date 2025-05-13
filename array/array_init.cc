
#include <array>
#include <iostream>

int main() {
  // class template argument deduction
  constexpr std::array data = {1.0, 2.3, 3.3, 4.5, 3.5};
  std::cout << "data size: " << data.size() << "\n";
  for (auto& x : data) {
    std::cout << x << " ";
  }
  std::cout << "\n";

  constexpr std::array flags = {false, false, true, false};
  std::cout << "flags size: " << flags.size() << "\n";
  for (auto& x : flags) {
    std::cout << x << " ";
  }
  std::cout << "\n";
}
