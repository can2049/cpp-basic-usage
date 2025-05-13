#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
using namespace std;

template <typename T>
void show(const T& data) {
  for (auto& d : data) {
    std::cout << d << ", ";
  }
  std::cout << std::endl;
}

int main() {
  set<int> data{1, 2, 3, 4, 5, 6};
  show(data);
  auto c = data.count(99);
  cout << c << endl;
}
