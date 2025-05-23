#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Data {
  mutable int a = 0;
  int b = 0;
};

int main() {
  int n1 = 0;           // non-const object
  const int n2 = 0;     // const object
  int const n3 = 0;     // const object (same as n2)
  volatile int n4 = 0;  // volatile object

  const struct {
    int n1;
    mutable int n2;
  } x = {0, 0};  // const object with mutable member

  n1 = 1;    // OK: modifiable object
             //  n2 = 2;   // error: non-modifiable object
  n4 = 3;    // OK: treated as a side-effect
             //  x.n1 = 4; // error: member of a const object is const
  x.n2 = 4;  // OK: mutable member of a const object isn't const

  const int& r1 = n1;  // reference to const bound to non-const object
  //  r1 = 2; // error: attempt to modify through reference to const
  const_cast<int&>(r1) = 2;  // OK: modifies non-const object n1

  const int& r2 = n2;  // reference to const bound to const object
  //  r2 = 2; // error: attempt to modify through reference to const
  //  const_cast<int&>(r2) = 2; // undefined behavior: attempt to modify const
  //  object n2

  [](...) {}(n3, n4, x, r2);  // see also: [[maybe_unused]]

  const Data d;
  d.a = 99;
  //   d.b = 999;

  char command[100] = "g++ -O3 -Wa,-adhln ";
  std::strcat(command, __FILE__);
  std::printf("cmd: %s\n", command);

  return std::system(command);  // may issue asm on POSIX systems
}
