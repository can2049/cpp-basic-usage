#include <array>
#include <boost/functional/hash.hpp>
#include <iostream>
#include <unordered_set>
#include <vector>

struct ArrayHash {
  template <class T, std::size_t N>
  size_t operator()(const std::array<T, N>& arr) const noexcept {
    return boost::hash_value(arr);
  }
};

std::unordered_set<std::array<int64_t, 2>, ArrayHash> create_set(
    std::vector<std::array<int64_t, 2>>& data) {
  std::unordered_set<std::array<int64_t, 2>, ArrayHash> hashs;
  for (auto& d : data) {
    hashs.insert(d);
  }
  return hashs;
}

int main() {
  std::vector<std::array<int64_t, 2>> data = {{{1, 2}, {2, 1}}};
  auto hashs = create_set(data);
  for (const auto& h : hashs) {
    std::cout << "{" << h[0] << ", " << h[1] << "} \n";
  }
  return 0;
}
