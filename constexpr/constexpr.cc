#include <iostream>

#include "timer.h"

using namespace std;

constexpr size_t factorial_const(size_t n) {
  if (n <= 1) {
    return 1;
  } else {
    return n * factorial_const(n - 1);
  }
}

size_t factorial_normal(size_t n) {
  if (n <= 1) {
    return 1;
  } else {
    return n * factorial_normal(n - 1);
  }
}

constexpr size_t fib_const(size_t n) {
  if (n <= 1) return n;
  return fib_const(n - 1) + fib_const(n - 2);
}

size_t fib_normal(size_t n) {
  if (n <= 1) return n;
  return fib_normal(n - 1) + fib_normal(n - 2);
}

int main() {
  for (size_t i = 0; i < 20; i++) {
    Timer t;
    auto f1 = fib_const(i);
    auto t1 = t.EndMilli(true);
    cout << i << " => " << f1 << "(" << t1 << ")";
    t.Start();
    auto f2 = fib_normal(i);
    auto t2 = t.EndMilli();
    cout << " | " << f2 << "(" << t2 << ")" << endl;
  }

  constexpr bool run = false;
  if constexpr (run) {
    cout << " hello !" << endl;
  }
}
