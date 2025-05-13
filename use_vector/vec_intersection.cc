#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
  std::vector<int> vec1 = {7, 2, 3, 4, 5};
  std::vector<int> vec2 = {4, 5, 6, 7, 8};

  // 确保输入的vector是有序的（如果需要的话）
  std::sort(vec1.begin(), vec1.end());
  std::sort(vec2.begin(), vec2.end());

  // 创建一个足够大的临时容器存放交集
  std::vector<int> intersection;
  intersection.reserve(std::min(vec1.size(), vec2.size()));

  // 使用set_intersection计算交集
  std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(),
                        std::back_inserter(intersection));

  // 修剪结果vector以去掉未使用的空间
  //   intersection.resize(std::distance(intersection.begin(), it));

  // 打印交集
  for (const auto &i : intersection) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  return 0;
}