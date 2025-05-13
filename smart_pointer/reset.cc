#include <iostream>
#include <memory>

void test_unique() {
  std::unique_ptr<int> u(new int(42));
  std::cout << "addr of u: " << u.get() << std::endl;
  u.reset();
  std::cout << "addr of u: " << u.get() << std::endl;  // nullptr
}

void test_shared() {
  std::shared_ptr<int> s(new int(42));
  std::cout << "addr of s: " << s.get() << std::endl;
  s.reset();
  std::cout << "addr of s: " << s.get() << std::endl;  // nullptr
}

void reset_nullptr() {
  std::unique_ptr<int> u = nullptr;
  u.reset();  // do nothing
  std::cout << "do nothing\n";
}

int main() {
  test_unique();
  test_shared();
  reset_nullptr();
  return 0;
}
