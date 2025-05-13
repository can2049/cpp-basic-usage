#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
void show(T& vv) {
  cout << "{";
  for (auto& v : vv) {
    cout << v << ", ";
  }
  cout << "}";
}

template <typename T>
void show_line(T& line) {
  for (auto& p : line) {
    show(p);
    cout << ", ";
  }
  cout << endl;
}

using BasicLine = std::vector<std::array<double, 3>>;

int main() {
  BasicLine line{{6, 5, 4}, {1, 6, 3}, {3, 4, 5}};
  show_line(line);
  std::sort(line.begin(), line.end());
  show_line(line);
  std::sort(line.begin(), line.end(),
            [](auto& a, auto& b) { return a[1] < b[1]; });
  show_line(line);
}