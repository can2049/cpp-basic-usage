#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "absl/types/span.h"

using namespace std;

template <typename T>
void show(const vector<T>& data) {
  for (auto& d : data) {
    cout << d << ", ";
  }
  cout << endl;
}

int main() {
  vector<int> data(5, 0);
  iota(data.begin(), data.end(), 0);
  show(data);
  absl::Span<int> d2(data);
  show(data);
}
