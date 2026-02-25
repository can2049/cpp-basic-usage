#include "absl/types/variant.h"

#include <cassert>
#include <iostream>
#include <string>

struct Fluid {};
struct LightItem {};
struct HeavyItem {};
struct FragileItem {};

struct VisitPackage {
  void operator()(Fluid&) { std::cout << "fluid\n"; }
  void operator()(LightItem&) { std::cout << "light item\n"; }
  void operator()(HeavyItem&) { std::cout << "heavy item\n"; }
  void operator()(FragileItem&) { std::cout << "fragile\n"; }
};

void visit_test() {
  absl::variant<Fluid, LightItem, HeavyItem, FragileItem> package{
      FragileItem()};

  // match with the current state in "package"
  absl::visit(VisitPackage(), package);
  std::cout << "=======visit test=======" << std::endl;
}

void hold_test() {
  absl::variant<int, std::string> v = "abc";
  std::cout << std::boolalpha << "variant holds int? "
            << absl::holds_alternative<int>(v) << '\n'
            << "variant holds string? "
            << absl::holds_alternative<std::string>(v) << '\n';
}

void access_test() {
  absl::variant<int, float> v, w;
  v = 42;  // v contains int
  int i = absl::get<int>(v);
  assert(42 == i);  // succeeds
                    //   float f = absl::get<float>(v);
                    //   std::cout << f << std::endl;
  w = absl::get<int>(v);
  w = absl::get<0>(v);  // same effect as the previous line
  w = v;                // same effect as the previous line

  //  absl::get<double>(v); // error: no double in [int, float]
  //  absl::get<3>(v);      // error: valid index values are 0 and 1

  try {
    absl::get<float>(w);  // w contains int, not float: will throw
  } catch (const absl::bad_variant_access& ex) {
    std::cout << ex.what() << '\n';
  }
}

void hold_test2() {
  using namespace std::literals;

  absl::variant<std::string> x("abc");
  // converting constructors work when unambiguous
  x = "def";  // converting assignment also works when unambiguous

  absl::variant<std::string, void const*> y("abc");
  // casts to void const * when passed a char const *
  assert(absl::holds_alternative<void const*>(y));  // succeeds
  y = "xyz"s;
  assert(absl::holds_alternative<std::string>(y));  // succeeds
}

int main() {
  visit_test();
  hold_test();
  access_test();
  hold_test2();
}
