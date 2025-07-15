
#include <iostream>
#include <iterator>
#include <map>

void find_value(const std::map<int, int>& values, int key) {
  auto it = values.find(key);
  if (it != values.end()) {
    std::cout << "Exactly found key: " << key << "\n";
    return;
  }
  const auto upper = values.upper_bound(key);
  const auto lower = std::prev(upper);

  std::cout << " No exact match for key: " << key << " upper==end? "
            << (upper == values.end()) << " lower==end? "
            << (lower == values.end()) << " upper==begin? "
            << (upper == values.begin()) << " lower==begin? "
            << (lower == values.begin()) << " lower==upper? "
            << (lower == upper) << " lower key: " << lower->first
            << " upper key: " << upper->first << "\n";
}

int main() {
  const std::map<int, int> values = {{1, 1}, {3, 3}};

  find_value(values, 0);
  find_value(values, 1);
  find_value(values, 2);
  find_value(values, 3);
  find_value(values, 4);
  return 0;
}
