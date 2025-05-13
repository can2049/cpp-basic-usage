// Online C++ compiler to run C++ program online
#include <array>
#include <iostream>
#include <vector>

void vector_zero() {
  std::vector<int> data0;
  std::cout << "vector. capacity: " << data0.capacity()
            << " size: " << data0.size() << std::endl;
  data0.resize(0);
  std::cout << "vector. capacity: " << data0.capacity()
            << " size: " << data0.size() << std::endl;
}

void c_array_zero() {
  int a[0];
  int b[0][100];
  int c[100][0];

  std::cout << "c array. sizeof(a) = " << sizeof(a) << " addr: " << (void*)a
            << std::endl;
  std::cout << "c array. sizeof(b) = " << sizeof(b) << " addr: " << (void*)b
            << std::endl;
  std::cout << "c array. sizeof(c) = " << sizeof(c) << " addr: " << (void*)c
            << std::endl;
}

void cpp_array_zero() {
  std::array<int, 0> a;
  std::cout << "c++ array. size of a: " << a.size() << std::endl;
}

int main() {
  // Write C++ code here

  vector_zero();
  c_array_zero();
  cpp_array_zero();
  return 0;

  std::vector<int> data;
  int sz = data.size() - 1;
  std::cout << "Hello world! sz: " << sz << std::endl;

  for (size_t i = 0; i < data.size() - 1; i++) {
    std::cout << "for loop. i: " << i << std::endl;
    if (i > 99) {
      break;
    }
  }

  return 0;
}
