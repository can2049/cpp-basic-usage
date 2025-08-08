#include <boost/tti/has_member_data.hpp>
#include <boost/tti/has_member_function.hpp>
#include <type_traits>

struct None {};

struct A {
  void foo() {}
};

BOOST_TTI_HAS_MEMBER_FUNCTION(foo)  // 生成模板 has_member_function_foo

static_assert(has_member_function_foo<A, void>::value, "A has foo()");
static_assert(!has_member_function_foo<None, void>::value, "None has no foo()");

struct C {
  int value;
};

BOOST_TTI_HAS_MEMBER_DATA(value)  // 生成模板 has_member_data_value

static_assert(has_member_data_value<C, int>::value, "C has int value");
static_assert(!has_member_data_value<None, int>::value,
              "None has no int value");

// custome check if a class has a member function
#define DEFINE_TYPE_TRAIT(name, func)                            \
  template <typename T>                                          \
  class name {                                                   \
   private:                                                      \
    template <typename Class>                                    \
    static char Test(decltype(&Class::func)*);                   \
    template <typename>                                          \
    static int Test(...);                                        \
                                                                 \
   public:                                                       \
    static constexpr bool value = sizeof(Test<T>(nullptr)) == 1; \
  };                                                             \
                                                                 \
  template <typename T>                                          \
  constexpr bool name<T>::value;

struct E {
  int x();
  void set_x(int x);
};

struct F {
  float x();
  void set_x(float x);
};

DEFINE_TYPE_TRAIT(has_x, x)
DEFINE_TYPE_TRAIT(has_set_x, set_x)

static_assert(has_x<E>::value, "E has x()");
static_assert(!has_x<None>::value, "None has no x()");
static_assert(has_set_x<E>::value, "E has set_x(int)");
static_assert(!has_set_x<None>::value, "None has no set_x(int)");
static_assert(has_x<E>::value, "E has x()");
static_assert(has_set_x<E>::value, "E has set_x(int)");

// no output is good
int main() { return 0; }
