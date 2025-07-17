#include <boost/icl/interval_set.hpp>
#include <iostream>

int main() {
  // 定义一个区间集合（自动合并重叠或相邻的区间）
  boost::icl::interval_set<int> merged_intervals;

  // 添加区间
  // [1, 3]
  merged_intervals += boost::icl::interval<int>::closed(1, 3);
  // [2, 5]（与 [1,3] 重叠，合并为 [1,5]）
  merged_intervals += boost::icl::interval<int>::closed(2, 5);
  // [7, 10]（不重叠，单独存储）
  merged_intervals += boost::icl::interval<int>::closed(7, 10);

  // 输出合并后的区间
  for (const auto& interval : merged_intervals) {
    std::cout << "[" << interval.lower() << ", " << interval.upper() << "] ";
  }
  // 输出: [1,5] [7,10]
  std::cout << std::endl;

  return 0;
}
