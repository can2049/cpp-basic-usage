#include <iostream>
#include <memory>

void func(std::shared_ptr<int>& p) { p = std::make_shared<int>(3); }

void func2(int* p) { *p = 4; }

int main() {
  std::shared_ptr<int> p = std::make_shared<int>(1);
  std::cout << "before func: " << *p << std::endl;
  func(p);
  std::cout << "after func: " << *p << std::endl;

  func2(p.get());
  std::cout << "after func2: " << *p << std::endl;

  return 0;
}
