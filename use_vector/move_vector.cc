#include <iostream>
#include <string>
#include <vector>

int main() {
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

  return 0;
}
