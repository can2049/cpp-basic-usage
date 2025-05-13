#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
void show(const vector<T>& data) {
  for (auto& d : data) {
    cout << d << ", ";
  }
  cout << endl;
}

void use_swap() {
  vector<int> data{1, 2, 3, 4};
  vector<int> data2{1, 2, 3, 4};
  show(data);
  data.swap(data2);
}

int main() {
  vector<int> data{1, 2, 3, 4};
  show(data);
  data.at(0) = 3;
  show(data);
  data.erase(data.begin(), data.begin() + 3);
  show(data);
}
