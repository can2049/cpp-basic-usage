#include <time.h>

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  srand(time(NULL));
  std::vector<int> vec{
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // 全是一样的元素，且必须超过16个元素
  std::sort(vec.begin(), vec.end(), [](const int& lhs, const int& rhs) {
    return lhs <= rhs;  // BUG，修改为: return lhs < rhs; 才行
  });
  std::cout << "end..." << rand() << std::endl;
  return 0;
}
