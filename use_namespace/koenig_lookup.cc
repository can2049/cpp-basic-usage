#include <iostream>

namespace math {
struct Vec3 {
  double x, y, z;
};

void print(const Vec3& v) {
  std::cout << v.x << ' ' << v.y << ' ' << v.z << '\n';
}
}  // namespace math

int main() {
  math::Vec3 p{1, 2, 3};
  print(p);  // 无需写 math::print(p)
}
