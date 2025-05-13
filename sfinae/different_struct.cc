#include <iostream>

class Base {
 public:
  int a;
};

template <bool UseRos = false>
class D : Base {
 public:
};

int main() { return 0; }
