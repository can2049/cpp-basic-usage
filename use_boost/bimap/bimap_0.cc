#include <boost/bimap.hpp>
#include <iostream>
#include <string>

int main() {
  // Define a bidirectional map with std::string as the key and int as the value
  boost::bimaps::bimap<std::string, int> bm;

  // Insert elements into the bimap
  bm.insert({"one", 1});
  bm.insert({"two", 2});
  bm.insert({"three", 3});
  // Allowed because the right view is a multiset
  bm.insert({"another_one", 1});

  // Access elements using the left map (key -> value)
  std::cout << "Value of 'one': " << bm.left.at("one") << std::endl;
  std::cout << "Value of 'two': " << bm.left.at("two") << std::endl;
  try {
    std::cout << "Value of 'another_one': " << bm.left.at("another_one")
              << std::endl;
  } catch (std::exception& e) {
    std::cerr << "Value of 'another_one' not found. err: " << e.what()
              << std::endl;
  }
  // Access elements using the right map (value -> key)
  std::cout << "Key of 3: " << bm.right.at(3) << std::endl;

  // Check if a key exists in the left map
  if (bm.left.find("four") == bm.left.end()) {
    std::cout << "'four' not found in the bimap." << std::endl;
  }

  // Iterate over the bimap
  std::cout << "Bimap contents:" << std::endl;
  for (const auto& [left, right] : bm) {
    std::cout << left << " <-> " << right << std::endl;
  }

  return 0;
}
