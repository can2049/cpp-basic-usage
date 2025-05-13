#include <iostream>

int main() {
#if SOME_MACRO_CANWANG - 0
  std::cout << "hello world" << std::endl;
#endif
  std::cout << "hello world out of macro" << std::endl;
}
