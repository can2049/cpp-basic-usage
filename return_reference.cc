#include <array>
#include <iostream>
#include <vector>

using namespace std;

struct Data {
  array<int, 4> a{};
  int b{};
  int& B() { return b; }
};

Data func(int b) {
  Data d;
  d.a = {b, b + 1, b + 2, b + 3};
  d.b = b * b;
  return d;
}

int main() {
  Data data;
  // data.a = {1, 2, 3, 4};
  data.b = 33;
  cout << data.b << endl;
  auto& bb = data.B();
  bb = 44;
  cout << data.b << endl;
}
