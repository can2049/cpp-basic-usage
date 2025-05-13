#include <iostream>
#include <memory>

void assign(std::shared_ptr<int>* ptr) { *ptr = std::make_shared<int>(9); }

int main() {
  std::shared_ptr<int> ptr = nullptr;
  std::cout << *ptr << '\n';  // nullptr
  assign(&ptr);
  std::cout << *ptr << '\n';  // 输出 9
}
