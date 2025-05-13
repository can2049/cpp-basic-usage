#include <iostream>

// 基类
class Base {
 public:
  virtual void func_a() { std::cout << "Base::func_a() called" << std::endl; }

  void func_b() {
    std::cout << "Base::func_b() called" << std::endl;
    func_a();  // 调用 func_a
  }
};

// 派生类
class Derived : public Base {
 public:
  void func_a() override {
    std::cout << "Derived::func_a() called" << std::endl;
  }
};

int main() {
  Derived derived;
  Base* basePtr = &derived;  // 基类指针指向派生类对象

  basePtr->func_b();  // 调用 func_b，func_b 内部调用 func_a

  return 0;
}
