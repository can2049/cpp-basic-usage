#include <iostream>
#include <memory>
#include <vector>
using namespace std;

int main() {
  auto pu = std::make_unique<std::vector<int>>();
  auto ps = std::make_shared<std::vector<int>>();
  // std::cout << p-> << std::endl;
  //   std::cout << p-> << std::endl;
  std::cout << ps.use_count() << std::endl;
}
