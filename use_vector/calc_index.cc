
#include <iostream>
#include <vector>

int get_index(const std::vector<int>& data, int value) {
  for (auto it = data.rbegin(); it != data.rend(); it++) {
    if (*it == value) {
      return std::distance(it, data.rend()) - 1;
    }
  }
  return 0;
}
int main() {
  std::vector data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto index = get_index(data, 9);
  std::cout << index << std::endl;
}
