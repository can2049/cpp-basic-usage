#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v1 = {1, 2, 3, 4};
  std::vector<int> v2 = {1, 2, 3, 4};

  if (v1.size() == v2.size() && std::equal(v1.begin(), v1.end(), v2.begin())) {
    std::cout << "The containers are equal." << std::endl;
  } else {
    std::cout << "The containers are not equal." << std::endl;
  }

  return 0;
}
