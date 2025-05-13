#include <iostream>
using namespace std;
int main() {
  const int a = 333;
  const int* p = &a;
  const int b = 4444;
  p = &b;
  cout << *p << endl;
}