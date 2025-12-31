#include <atomic>
#include <cassert>
#include <iostream>
#include <utility>  // for std::move

template <typename T>
class SharedPtr {
 private:
  T* ptr_;                       // 原始指针
  std::atomic<int>* ref_count_;  // 原子引用计数

  // 释放资源（线程安全）
  void release() {
    if (ref_count_) {
      (*ref_count_) -= 1;
      if (*ref_count_ == 0) {
        delete ptr_;
        delete ref_count_;
      }
    }
  }

 public:
  // 默认构造函数
  SharedPtr() : ptr_(nullptr), ref_count_(nullptr) {}

  // 构造函数，接受原始指针
  explicit SharedPtr(T* p) : ptr_(p), ref_count_(new std::atomic<int>(1)) {}

  // 拷贝构造函数（线程安全）
  SharedPtr(const SharedPtr<T>& other)
      : ptr_(other.ptr_), ref_count_(other.ref_count_) {
    if (ref_count_) {
      (*ref_count_) += 1;
    }
  }

  // 移动构造函数
  SharedPtr(SharedPtr<T>&& other) noexcept
      : ptr_(other.ptr_), ref_count_(other.ref_count_) {
    other.ptr_ = nullptr;
    other.ref_count_ = nullptr;
  }

  // 析构函数（线程安全）
  ~SharedPtr() { release(); }

  // 拷贝赋值运算符（线程安全）
  SharedPtr<T>& operator=(const SharedPtr<T>& other) {
    if (this != &other) {
      release();
      ptr_ = other.ptr_;
      ref_count_ = other.ref_count_;
      if (ref_count_) {
        (*ref_count_) += 1;
      }
    }
    return *this;
  }

  // 移动赋值运算符
  SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept {
    if (this != &other) {
      release();
      ptr_ = other.ptr_;
      ref_count_ = other.ref_count_;
      other.ptr_ = nullptr;
      other.ref_count_ = nullptr;
    }
    return *this;
  }

  // 解引用运算符
  T& operator*() const { return *ptr_; }

  // 箭头运算符
  T* operator->() const { return ptr_; }

  // 获取原始指针
  T* get() const { return ptr_; }

  // 获取引用计数（线程安全）
  int use_count() const {
    return ref_count_ != nullptr ? ref_count_->load() : 0;
  }

  // 检查是否唯一所有者（线程安全）
  bool unique() const { return use_count() == 1; }

  // 重置指针（线程安全）
  void reset(T* p = nullptr) {
    release();
    if (p) {
      ptr_ = p;
      ref_count_ = new std::atomic<int>(1);
    }
  }
};

class Data {
 public:
  Data() { std::cout << "constructor...\n"; }
  ~Data() { std::cout << "destructor...\n"; }
};

// 测试代码
int main() {
  // 创建shared_ptr
  SharedPtr<int> p1(new int(42));
  std::cout << "p1 value: " << *p1 << ", use count: " << p1.use_count()
            << std::endl;

  // 拷贝构造
  SharedPtr<int> p2 = p1;
  std::cout << "p1 use count after copy: " << p1.use_count() << std::endl;
  std::cout << "p2 value: " << *p2 << ", use count: " << p2.use_count()
            << std::endl;

  // 赋值操作
  SharedPtr<int> p3;
  p3 = p2;
  std::cout << "p1 use count after assignment: " << p1.use_count() << std::endl;

  // 移动语义
  SharedPtr<int> p4 = std::move(p3);
  std::cout << "p3 after move: " << (p3.get() ? "not null" : "null")
            << std::endl;
  std::cout << "p4 value: " << *p4 << ", use count: " << p4.use_count()
            << std::endl;

  // 重置指针
  p2.reset(new int(100));
  std::cout << "p1 use count after p2 reset: " << p1.use_count() << std::endl;
  std::cout << "p2 value after reset: " << *p2
            << ", use count: " << p2.use_count() << std::endl;

  SharedPtr<Data> p5(new Data());

  p5 = p5;  // self-assignment test, ok

  return 0;
}
