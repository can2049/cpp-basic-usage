#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename T>
void show(const vector<T>& data) {
  for (auto& d : data) {
    cout << d << ", ";
  }
  cout << endl;
}

int main() {
  vector<int> data(10);
  std::iota(data.begin(), data.end(), 0);
  show(data);
  int count = 0;
  for (auto i = data.begin(); i != data.end(); count++, ++i) {
    cout << " idx: " << distance(data.begin(), i) << " content: " << *i
         << "  count: " << count << endl;
    if (*i == 0 || *i == 3 || *i == 4 || *i == 6) {
      i = data.erase(i);
      cout << " i-> " << *i << endl;
      --i;
      continue;
    }
  }
  show(data);
}
