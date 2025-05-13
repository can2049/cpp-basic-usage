// too many product operation
#include <stdlib.h>

#include "log.h"

int product_test(int a, int b) {
  int c1 = a * b + 1;
  int c2 = a * b + 2;
  int c3 = a * b + 3;
  int c4 = a * b + 4;
  int c5 = a * b + 5;
  LOG(INFO) << "===========" << c1 << c2 << c3 << c4 << c5;
  return c1 + c2 + c3 + c4 + c5;
}

int main() {
  init_log();

  int a = rand();
  int b = rand();

  product_test(a, b);

  return 0;
}
