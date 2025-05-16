#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>

int main() {
  std::random_device rd{};
  std::mt19937 gen(rd());

  // values near the mean are the most likely
  // standard deviation affects the dispersion of generated values from the mean
  std::normal_distribution d{5.0, 2.0};

  // draw a sample from the normal distribution and round it to an integer
  auto random_int = [&d, &gen] { return std::round(d(gen)); };

  std::map<int, int> hist{};
  for (int n = 0; n != 10000; ++n) ++hist[random_int()];

  for (auto [x, y] : hist)
    std::cout << std::setw(2) << x << ' ' << std::string(y / 200, '*') << '\n';
}
