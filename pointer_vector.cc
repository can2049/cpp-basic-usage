#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
// using namespace std;

struct Data {
  boost::shared_ptr<std::unordered_map<int, int>> imgs{};
};

void show(Data& data) {
  std::cout << "data size: " << data.imgs->size() << std::endl;
  for (auto& d : *(data.imgs)) {
    std::cout << d.first << ", ";
  }
  std::cout << std::endl;
}

int main() {
  auto data = Data();
  data.imgs = boost::make_shared<std::unordered_map<int, int>>();
  show(data);
  data.imgs->insert({3, 4});
  show(data);
  data.imgs->clear();
  show(data);
}