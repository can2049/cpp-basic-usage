#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
  os << '{';
  int n = v.size();
  for (auto const& e : v) {
    os << e << (--n ? ", " : "");
  }
  return os << '}';
}

struct Order  // a struct with very interesting data
{
  int order_id{};

  friend std::ostream& operator<<(std::ostream& os, const Order& ord) {
    return os << ord.order_id;
  }
};

// using namespace std;

void show_vec_str_diff() {
  std::vector<std::string> we_got = {"traffic_2", "camera_5", "camera_3",
                                     "camera_1",  "camera_4", "camera_2"};
  std::vector<std::string> we_need = {"traffic_2", "camera_5", "camera_3",
                                      "camera_1",  "camera_4", "camera_2",
                                      "camera_6",  "camera_7"};
  std::vector<std::string> no_arrived;
  // get element not in first set
  std::set_difference(we_need.begin(), we_need.end(), we_got.begin(),
                      we_got.end(),
                      std::inserter(no_arrived, no_arrived.end()));

  std::cout << we_need << " ∖ " << we_got << " == " << no_arrived << "\n\n";
}

int show_vec_str_diff2() {
  std::set<std::string> a;
  std::set<std::string> b;
  std::set<std::string> result;

  a.insert("one");
  a.insert("two");
  a.insert("three");

  b.insert("a");
  b.insert("b");
  b.insert("three");

  std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                      inserter(result, result.begin()));

  std::cout << "\nDifference" << std::endl << "-------------\n";

  for (std::set<std::string>::const_iterator i = result.begin();
       i != result.end(); ++i) {
    std::cout << *i << std::endl;
  }

  result.clear();
  std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                inserter(result, result.begin()));

  std::cout << "\nSymmetric Difference\n-------------\n";

  for (std::set<std::string>::const_iterator i = result.begin();
       i != result.end(); ++i) {
    std::cout << *i << std::endl;
  }

  return 0;
}

int main() {
  const std::vector<int> v1{1, 2, 5, 5, 5, 9};
  const std::vector<int> v2{2, 5, 7};
  std::vector<int> diff;

  std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                      std::inserter(diff, diff.begin()));

  std::cout << v1 << " ∖ " << v2 << " == " << diff << "\n\n";

  // we want to know which orders "cut" between old and new states:
  std::vector<Order> old_orders{{1}, {2}, {5}, {9}};
  std::vector<Order> new_orders{{2}, {5}, {7}};
  std::vector<Order> cut_orders;

  std::set_difference(old_orders.begin(), old_orders.end(), new_orders.begin(),
                      new_orders.end(), std::back_inserter(cut_orders),
                      [](auto& a, auto& b) { return a.order_id < b.order_id; });

  std::cout << "old orders: " << old_orders << '\n'
            << "new orders: " << new_orders << '\n'
            << "cut orders: " << cut_orders << '\n';

  show_vec_str_diff();
  show_vec_str_diff2();
}
