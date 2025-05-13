#include <atomic>
#include <iostream>

using namespace std;

class Data {
 public:
  Data() { cout << "constructor...\n"; }
  ~Data() { cout << "destructor...\n"; }
};

template <typename T>
class SharedPtr {
 public:
  SharedPtr(T* ptr = nullptr) : ptr_(ptr) { count_ = new atomic<int>(1); }

  SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), count_(other.count_) {
    if (count_) {
      ++(*count_);
    }
  }

  SharedPtr& operator=(const SharedPtr& other) {
    // cout << "1. " << *count_ << ", ptr_ = " << ptr_ << endl;
    if (this != &other) {
      release();

      ptr_ = other.ptr_;
      count_ = other.count_;

      if (count_) {
        count_->fetch_add(1);
      }
    }

    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept
      : ptr_(other.ptr_), count_(other.count_) {
    cout << "13. " << *count_ << ", ptr_ = " << ptr_ << endl;
    other.ptr_ = nullptr;
    other.count_ = nullptr;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    cout << "12. " << *count_ << ", ptr_ = " << ptr_ << endl;
    if (this != &other) {
      release();

      ptr_ = other.ptr_;
      count_ = other.count_;

      other.ptr_ = nullptr;
      other.count_ = nullptr;
    }

    return *this;
  }

  ~SharedPtr() { release(); }

  int count() { return count_->load(); }

  void release() {
    // cout << "1. " << *count_ << ", ptr_ = " << ptr_ << endl;

    count_->fetch_sub(1);
    if (ptr_ && count_ && count_->load(std::memory_order_acquire) == 0) {
      // cout << "2. " << *count_ << endl;

      delete ptr_;
      delete count_;

      ptr_ = nullptr;
      count_ = nullptr;
    }
  }

 private:
  T* ptr_;
  std::atomic<int>* count_;
};

int main() {
  {
    SharedPtr<int> p1(new int(10));
    SharedPtr<Data> p2(new Data());
    auto p3 = p2;

    cout << "1. count = " << p3.count() << ", count = " << p2.count() << endl;
    auto p4 = std::move(p3);
    cout << "2. count = " << p4.count() << endl;
  }
}

// 64 位输出请用 printf(\"%lld\")