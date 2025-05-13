#include <iostream>
#include <vector>

#include "timer.h"

void test(size_t sz) {
  std::vector<float> v_a;
  std::vector<float> v_b;
  std::cout << " vector size: " << sz;
  // push back
  Timer t1;
  v_a.reserve(sz);
  v_a.clear();
  for (size_t i = 0; i < sz; ++i) {
    float value = i / 255.0f;
    v_a.push_back(value);
  }
  auto elapsed1 = t1.EndMilli();
  std::cout << "   push_back: " << elapsed1;

  // operator []
  Timer t2;
  v_b.resize(sz, 0.0f);
  for (size_t i = 0; i < sz; ++i) {
    float value = i / 255.0f;
    v_a[i] = value;
  }
  auto elapsed2 = t2.EndMilli();
  std::cout << "    operator []: " << elapsed2 << std::endl;
}

int main() {
  size_t sz = 1000;
  for (int i = 0; i < 10; i++) {
    sz *= i + 1;
    test(sz);
  }
  return 0;
}