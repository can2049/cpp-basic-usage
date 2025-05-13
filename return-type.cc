#include <iostream>
using namespace std;

int foo(int c) {
  cout << "c->" << c << endl;
  return c + c;
}

int main() { foo(3); }
