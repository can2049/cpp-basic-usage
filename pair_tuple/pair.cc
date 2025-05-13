#include <iostream>
#include <utility>

std::pair<int, int> func() {
  int left = 3;
  int right = 4;
  auto ans = std::make_pair(left, right);
  left = 5;
  right = 6;
  return ans;
}

using namespace std;

int main() {
  const auto& [a, b] = func();
  cout << a << " , " << b << endl;
}