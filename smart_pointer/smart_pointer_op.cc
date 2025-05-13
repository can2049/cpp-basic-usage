#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<int> p1(new int(42));
  std::shared_ptr<int> p2 = p1;
  std::cout << *p1 << " " << *p2 << std::endl;
  std::cout << !p1 << "\n";
}
