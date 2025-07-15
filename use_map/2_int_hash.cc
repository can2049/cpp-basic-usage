#include <array>
#include <boost/functional/hash.hpp>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_set>
#include <vector>

std::vector<std::array<int64_t, 2>> generateRandomData(size_t count,
                                                       int64_t min_value,
                                                       int64_t max_value) {
  std::vector<std::array<int64_t, 2>> result;
  result.reserve(count);

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dis(min_value, max_value);

  for (size_t i = 0; i < count; ++i) {
    result.push_back({dis(gen), dis(gen)});
  }

  return result;
}

struct ArrayHash {
  template <class T, std::size_t N>
  size_t operator()(const std::array<T, N>& arr) const noexcept {
    return boost::hash_value(arr);
  }
};

std::unordered_set<std::array<int64_t, 2>, ArrayHash> create_set(
    std::vector<std::array<int64_t, 2>>& data) {
  std::unordered_set<std::array<int64_t, 2>, ArrayHash> hashs;
  hashs.reserve(data.size());
  for (auto& d : data) {
    auto result = hashs.insert(d);
    if (!result.second) {
      // insert failed, meaning the element already exists
      const auto& old = *(result.first);
      if (old == d) {
        continue;  // No need to print if they are the same
      }
      std::cout << "duplicated!";
      std::cout << " old: [" << old[0] << ", " << old[1] << "]";
      std::cout << " new: [" << d[0] << ", " << d[1] << "]";
      std::cout << " Hash: " << ArrayHash{}(d);
      std::cout << "\n";
    }
  }
  return hashs;
}

void simple_test() {
  std::vector<std::array<int64_t, 2>> data = {{{1, 2}, {2, 1}}};
  auto hashs = create_set(data);
  for (const auto& h : hashs) {
    std::cout << "{" << h[0] << ", " << h[1] << "} \n";
  }
}

void big_test(size_t count,
              int64_t min_value = std::numeric_limits<int64_t>::min(),
              int64_t max_value = std::numeric_limits<int64_t>::max()) {
  std::cout << " Running big test with count: " << count
            << " min_value: " << min_value << " max_value: " << max_value
            << "\n";

  auto data = generateRandomData(count, min_value, max_value);
  auto hashs = create_set(data);
  std::cout << "data size: " << data.size()
            << " size diff: " << data.size() - hashs.size() << "\n";
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    simple_test();
    return 0;
  }

  if (argc == 2) {
    size_t count = std::stod(argv[1]);

    big_test(count);

    return 0;
  }

  if (argc == 4) {
    size_t count = std::stod(argv[1]);
    int64_t min_value = std::stod(argv[2]);
    int64_t max_value = std::stod(argv[3]);
    big_test(count, min_value, max_value);
    return 0;
  }

  std::cout << "Usage: " << argv[0] << " [count] [min_value] [max_value]\n"
            << "Example: " << argv[0] << " 1000000 -1000000 1000000\n";
  std::cout << "Example: " << argv[0] << " 1000000\n";

  return 0;
}
