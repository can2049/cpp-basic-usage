#include <stdio.h>
#include <stdlib.h>

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return -1;
  }
  int a;
  /* Get the value from somewhere GCC can't optimize */
  a = atoi(argv[1]);
  if (likely(a == 2)) {
    printf("equal: %d\n", a);
  } else {
    printf("not equal:%d\n", a);
  }

  printf("%d\n", a);

  return 0;
}
