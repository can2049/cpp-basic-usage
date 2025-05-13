#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

// using namespace std;

// 计算并检查结果是否为24
class Solution {
 public:
  bool judgePoint24(std::vector<int> &nums) {
    std::vector<double> nums_f;
    for (const int &num : nums) {
      nums_f.emplace_back(static_cast<double>(num));
    }
    return solve(nums_f, "");
  }

  bool solve(const std::vector<double> &nums, const std::string &expr) {
    if (nums.empty()) {
      return false;
    }
    if (nums.size() == 1) {
      if (fabs(nums[0] - kTarget) < kEpsilon) {
        std::cout << expr << "\n";
        return true;
      }
    }
    int size = nums.size();
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (i == j) {
          continue;
        }
        std::vector<double> list2;
        for (int k = 0; k < size; k++) {
          if (k != i && k != j) {
            list2.emplace_back(nums[k]);
          }
        }
        for (auto op : kOperators) {
          if ((op == kAdd || op == kMultiply) && i > j) {
            continue;
          }
          std::string new_expr = expr + " --> (" +
                                 std::to_string((int)nums[i]) + op +
                                 std::to_string((int)nums[j]) + ")";
          //   std::string new_expr;
          //   if (expr.empty()) {
          //     new_expr = "(" + std::to_string((int)nums[i]) + op +
          //                std::to_string((int)nums[j]) + ")";
          //   } else {
          //     new_expr = std::string("(") + std::to_string((int)nums[j]) + op
          //     +
          //                expr + ")";
          //   }

          if (op == kAdd) {
            list2.emplace_back(nums[i] + nums[j]);
          } else if (op == kMultiply) {
            list2.emplace_back(nums[i] * nums[j]);
          } else if (op == kSubtract) {
            if (nums[i] < nums[j]) {
              continue;
            }
            list2.emplace_back(nums[i] - nums[j]);
          } else if (op == kDivide) {
            if (fabs(nums[j]) < kEpsilon) {
              continue;
            }
            list2.emplace_back(nums[i] / nums[j]);
          }
          if (solve(list2, new_expr)) {
            return true;
          }
          list2.pop_back();
        }
      }
    }
    return false;
  }

 private:
  static constexpr int kTarget = 24;
  static constexpr double kEpsilon = 1e-6;
  static constexpr char kAdd = '+';
  static constexpr char kMultiply = '*';
  static constexpr char kSubtract = '-';
  static constexpr char kDivide = '/';
  static constexpr char kOperators[4] = {kAdd, kMultiply, kSubtract, kDivide};
  std::set<std::string> expr_set_;
};

int main() {
  std::vector<int> cards = {8, 12, 7, 8};  // 示例输入
  if (Solution().judgePoint24(cards)) {
    std::cout << "true\n";
  } else {
    std::cout << "false\n";
  }
  return 0;
}
