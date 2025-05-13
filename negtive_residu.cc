#include <iostream>
using namespace std;

bool BoundaryShouldLight(float x) {
  if (x >= 0) {
    auto res = (int)x % 4;
    return res == 0 || res == 1;
  }
  x = -x;
  auto res = (int)x % 4;
  return res == 2 || res == 3;
}

int main() {
  for (float i = -10; i < 10; i += 0.5) {
    cout << i << "\t->\t" << BoundaryShouldLight(i) << endl;
  }
}