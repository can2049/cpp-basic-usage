#include <iostream>
#include <queue>

class Data {
 public:
  Data(int id) : id_(id) { std::cout << " constructor... id: " << id << "\n"; }
  ~Data() { std::cout << " destructor...id: " << id_ << "\n"; }
  Data(const Data& other) { std::cout << " copy constructor...\n"; }
  Data& operator=(const Data& other) {
    std::cout << " copy assignment...\n";
    return *this;
  }
  Data(Data&& other) { std::cout << " move constructor...\n"; }
  Data& operator=(Data&& other) {
    std::cout << " move assignment...\n";
    return *this;
  }
  void doSomething() { std::cout << " doing something...\n"; }

  int id_ = -1;
};

int main() {
  {
    std::queue<Data> q;
    q.emplace(1);  // 调用移动构造函数
    q.emplace(2);  // 调用移动构造函数
    std::cout << "--------\n";
  }

  std::cout << "=======\n";

  return 0;
}
