#include <absl/strings/str_join.h>

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

template <typename T>
std::vector<T> RemoveElementFromSet(const std::unordered_set<T>& remove_set,
                                    std::vector<T>& vec) {
  std::vector<T> removed;
  removed.reserve(remove_set.size());
  // Use remove_if and erase to remove elements that are in the set
  vec.erase(std::remove_if(vec.begin(), vec.end(),
                           [&remove_set, &removed](T& x) {
                             if (remove_set.find(x) != remove_set.end()) {
                               removed.push_back(x);
                               return true;
                             }
                             return false;
                           }),
            vec.end());
  return removed;
}

int main() {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 5, 4, 3, 2};
  std::unordered_set<int> s = {1, 2, 3};
  std::cout << "vec: " << absl::StrJoin(vec, ",") << std::endl;
  std::cout << "remove set: " << absl::StrJoin(s, ",") << std::endl;
  auto removed = RemoveElementFromSet(s, vec);
  std::cout << "after remove\n";
  std::cout << "vec: " << absl::StrJoin(vec, ",") << std::endl;
  std::cout << "removed: " << absl::StrJoin(removed, ",") << std::endl;
}
