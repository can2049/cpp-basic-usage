#include <iostream>
#include <vector>

class Base {
 public:
  virtual void sound() = 0;
};

class Cat : public Base {
 public:
  void sound() { std::cout << "meow" << std::endl; }
};

class Dog : public Base {
 public:
  void sound() { std::cout << "woof" << std::endl; }
};

int main() {
  std::vector<Base*> animals;

  animals.emplace_back(new Cat());
  animals.emplace_back(new Dog());

  for (const auto base : animals) {
    base->sound();
  }

  return 0;
}
