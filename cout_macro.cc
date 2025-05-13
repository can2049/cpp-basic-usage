
#include <iostream>
// One level of macro indirection is required in order to resolve __COUNTER__,
// and get varname1 instead of varname__COUNTER__.
// #define CONCAT_(x, y) x##y
// #define CONCAT(x, y) CONCAT_(x, y)
// #define uniquename static bool CONCAT(_name_, __COUNTER__) = false
int func(int c) { return c + 1; }

#if 0
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b

#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

#define CHECK(value)           \
  auto CONCAT_INNER = (value); \
  if (CONCAT_INNER != 0) std::cout << #value << " returned: " << CONCAT_INNER

int main() {
  // int CONCAT_INNER = 123;
  // std::cout << foo0 << std::endl;  // prints "123"
  CHECK(func(3));
  CHECK(func(0));
}
#endif

#define CHECK(value)          \
  auto UNIQUE_NAME = (value); \
  if (UNIQUE_NAME != 0) std::cout << #value << " returned: " << UNIQUE_NAME

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b

#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

// int func(int c) {
//   auto UNIQUE_NAME(aaa) = c;
//   return UNIQUE_NAME(aaa);
// }

int main() {
  // int UNIQUE_NAME(foo) = 123;  // int foo0 = 123;
  // int UNIQUE_NAME(foo) = 122;  // int foo0 = 123;
  // int UNIQUE_NAME(foo) = 121;  // int foo0 = 123;
  // CHECK(func(3));
  // CHECK(func(0));
  auto a = 3;
  if (a < 4) {
    std::cout << a << std::endl;
  }
}
