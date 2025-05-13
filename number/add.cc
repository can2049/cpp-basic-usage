#include <iostream>

int main() {
  {
    int a = 0;
    int b = --a;
    std::cout << "--a. a: " << a << " b: " << b << "\n";
  }
  {
    int a = 0;
    int b = ++a;
    std::cout << "++a. a: " << a << " b: " << b << "\n";
  }
  {
    int a = 0;
    int b = a--;
    std::cout << "a--. a: " << a << " b: " << b << "\n";
  }
  {
    int a = 0;
    int b = a++;
    std::cout << "a++. a: " << a << " b: " << b << "\n";
  }
}
