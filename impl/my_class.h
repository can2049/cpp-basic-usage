#pragma once
#include <memory>
class MyClass {
 public:
  MyClass();
  ~MyClass();

  void Init(int a);

  void doSomething();

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl_;
};
