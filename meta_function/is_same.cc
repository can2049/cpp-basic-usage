#include <iostream>

template <typename T, typename U>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static const bool value = true;
};

template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

template <typename T, T v>
struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

int main() {
  using int_1_t = integral_constant<int, 1>;
  using int_2_t = integral_constant<int, 2>;

  std::cout << std::boolalpha
            << is_same_v<int_1_t, int_2_t> << std::endl;  // false
  std::cout << std::boolalpha
            << is_same_v<int_1_t::type, int_2_t::type> << std::endl;  // false
  std::cout << std::boolalpha
            << is_same_v<int_1_t::value_type,
                         int_2_t::value_type> << std::endl;  // true

  int a = int_1_t();  // call constexpr operator value_type()
  std::cout << "a: " << a << std::endl;

  int_1_t int_1_o1;  // 定义两个类型和值都一样的变量，就像 int a = 1; int b = 1;
                     // 一样
  int_1_t int_1_o2;

  int b = int_1_o1();  // call constexpr value_type operator()()
  std::cout << "b: " << b << std::endl;

  std::cout << std::boolalpha << (int_1_o1 == int_1_o2);  // true

  return 0;
}
