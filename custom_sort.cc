#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
void show(const T& data) {
  for (auto& d : data) {
    std::cout << d << ", ";
  }
  std::cout << std::endl;
}

int main() {
  vector<int> data{10, 8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
  show(data);
  std::sort(data.begin(), data.end(), [&](auto& a, auto& b) { return a < b; });
  show(data);
}
