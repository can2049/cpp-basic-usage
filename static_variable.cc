#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void func() {
  const static vector<int> data1{1, 2, 3, 4};
  const vector<int> data2{1, 2, 3, 4, 5, 6, 7, 8, 9};
  cout << "addr1: " << (void*)&data1[2] << " addr2: " << (void*)&data2[2]
       << endl;
}

int main() {
  for (auto i = 0; i < 5; i++) {
    func();
  }
}
