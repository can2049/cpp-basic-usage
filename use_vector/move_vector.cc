#include <iostream>
#include <optional>
#include <string>
#include <vector>

void test_move_element() {
  std::vector<std::string> source = {"hello", "world"};
  std::optional<std::string> dest;

  // same effect
  dest = std::move(source[0]);
  // dest->swap(source[0]);

  for (auto& s : source) {
    std::cout << "source element: '" << s << "'\n";
  }
  std::cout << "dest element: '" << *dest << "'\n";
}

void test_move_vector() {
  // Example source vector
  std::vector<std::string> source = {"1", "2", "3", "4", "5"};

  // Create a destination vector and move all elements from source to
  // destination
  std::vector<std::string> destination = {"a", "b", "c"};
  destination.insert(destination.end(), std::make_move_iterator(source.begin()),
                     std::make_move_iterator(source.end()));

  // Print the elements of the destination vector to verify
  std::cout << "Destination vector:";
  for (auto num : destination) {
    std::cout << " " << num;
  }
  std::cout << std::endl;

  // Print the elements of the source vector (should be empty after moving)
  std::cout << "Source vector:";
  for (auto num : source) {
    std::cout << " " << num;
  }
  std::cout << std::endl;
}

int main() {
  test_move_element();
  test_move_vector();
  return 0;
}
