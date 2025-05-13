
#include <Eigen/Core>
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

template <typename T>
void show(T& data) {
  std::cout << "{";
  for (auto& d : data) {
    std::cout << d << ", ";
  }
  std::cout << "}";
}

int main() {
  std::vector<float> data(16);
  std::iota(data.begin(), data.end(), 0);
  show(data);
  std::cout << std::endl;
  Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::RowMajor>> M(data.data());
  std::cout << M << std::endl;

  Eigen::Matrix<float, 4, 4, Eigen::RowMajor> m2;
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
  std::cout << " m2: " << m2 << "\n";

  auto a = m2.size();

  return 0;
}
