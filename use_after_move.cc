#include <iostream>
#include <memory>
int main() {
  std::cout << "sizeof(bool) " << sizeof(bool) << std::endl;
  auto a = std::make_shared<std::string>("hello");
  std::cout << "a: " << *a << std::endl;
  auto b = std::move(a);
  std::cout << "a: " << *a << std::endl;
}
