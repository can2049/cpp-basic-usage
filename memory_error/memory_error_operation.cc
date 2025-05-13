#include <iostream>

int main() {
  int *p = new int[5];
  for (int i = 0; i < 5; i++) {
    p[i] = i;
  }
  for (int i = 0; i < 8; i++) {
    std::cout << p[i] << " ";
  }
  //   delete p;
  delete[] p;
  delete[] p;
  return 0;
}
