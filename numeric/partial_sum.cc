#include <iostream>
#include <numeric>
#include <vector>

int main() {
  std::vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::cout << "input vector:          ";
  for (auto i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::vector<int> partial_sums{};
  partial_sums.reserve(v.size());
  std::vector<int> exclusion_scan_results{};
  exclusion_scan_results.reserve(v.size());
  std::partial_sum(v.begin(), v.end(), std::back_inserter(partial_sums));
  std::exclusive_scan(v.begin(), v.end(),
                      std::back_inserter(exclusion_scan_results), 0,
                      std::plus<int>());
  std::cout << "partial_sum results   :";
  for (auto ps : partial_sums) {
    std::cout << ps << " ";
    ;
  }
  std::cout << std::endl;
  std::cout << "exclusive_scan results:";
  for (auto ps : exclusion_scan_results) {
    std::cout << ps << " ";
    ;
  }

  std::vector<int> inclusion_scan_results{};
  inclusion_scan_results.reserve(v.size());

  std::inclusive_scan(v.begin(), v.end(),
                      std::back_inserter(inclusion_scan_results));
  std::cout << std::endl;
  std::cout << "inclusive_scan results:";
  for (auto ps : inclusion_scan_results) {
    std::cout << ps << " ";
    ;
  }

  std::cout << std::endl;
}
