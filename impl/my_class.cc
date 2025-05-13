#include "my_class.h"

#include <iostream>
using namespace std;
struct MyClass::Impl {
  int a_;
  int b_;
  int c_;
};

MyClass::MyClass() : pImpl_(std::make_unique<Impl>()) {}

MyClass::~MyClass() {}

void MyClass::doSomething() {
  // Do something that the implementation and users of MyClass should be able to
  // do
  std::cout << "Doing something from MyClass!" << std::endl;
}

void MyClass::Init(int a) {
  pImpl_->a_ = a;
  cout << "init" << endl;
}
