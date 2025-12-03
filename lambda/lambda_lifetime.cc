#include <iostream>
#include <memory>

class Processor {
 private:
  std::unique_ptr<int> smart_ptr_;
  int* ptr_;

 public:
  auto dangerous_lambda() {
    return [=]() {
      std::cout << " raw ptr value: " << *ptr_ << " \n";
      std::cout << " smart ptr value: " << *smart_ptr_ << " \n";
    };
  }

  Processor(int val) {
    smart_ptr_ = std::make_unique<int>(val);
    ptr_ = new int(val);
  }

  ~Processor() {
    std::cout << "Processor is being destroyed" << std::endl;

    delete ptr_;
    ptr_ = nullptr;
  }
};

int main() {
  auto p = std::make_unique<Processor>(42);
  auto func = p->dangerous_lambda();

  func();  // ok

  p = nullptr;

  std::cout << " after manual destruction" << std::endl;

  func();  // 很可能崩溃！因为访问了已释放的内存
}
