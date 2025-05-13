// inside for-loop, clear container
#include <iostream>
#include <vector>

int main() {
  std::vector<int> data = {1, 2, 3};
  for (auto& d : data) {
    std::cout << " d-> " << d << std::endl;
    data.clear();
  }
}
