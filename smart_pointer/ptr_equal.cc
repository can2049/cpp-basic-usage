#include <iostream>
#include <memory>

int main() {
  auto p1 = std::make_shared<int>(1);
  auto p2 = p1;
  auto p3 = std::make_shared<int>(3);

  std::cout << "p1 == p2 ? " << (p1 == p2) << std::endl;
  std::cout << "p1 == p3 ? " << (p1 == p3) << std::endl;
  std::cout << "p1 > p3 ? " << (p1 > p3) << std::endl;
  std::cout << "p1 < p3 ? " << (p1 < p3) << std::endl;
}
