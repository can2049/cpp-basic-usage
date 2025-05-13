#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>  // for std::pair

// 自定义哈希函数
struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ (hash2 << 1);  // 或者使用其他组合方式
  }
};

int main() {
  std::unordered_map<std::pair<int, int>, std::string, PairHash> myMap;

  // 插入元素
  myMap[std::make_pair(1, 2)] = "value_1_2";
  myMap[std::make_pair(2, 1)] = "value_2_1";
  myMap[{3, 4}] = "value_3_4";  // C++11起支持的统一初始化语法

  // 访问元素
  std::cout << " key: {1,2}, value: " << myMap[{1, 2}]
            << std::endl;  // 输出: value1

  // 遍历map
  for (const auto& entry : myMap) {
    std::cout << "Key: (" << entry.first.first << ", " << entry.first.second
              << "), Value: " << entry.second << std::endl;
  }

  return 0;
}
