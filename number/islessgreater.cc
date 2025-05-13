#include <cmath>
#include <iostream>

int main() {
  double a = 3.143383809215656;
  double b = a;

  // Check if a is less than or greater than b
  if (std::islessgreater(a, b)) {
    std::cout << "a is either less than or greater than b" << std::endl;
  } else {
    std::cout << "a is equal to b" << std::endl;
  }

  return 0;
}
