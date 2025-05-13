#include <atomic>
#include <iostream>
#include <utility>  // for std::move

template <typename T>
class SharedPtr {
 private:
  T* ptr;                       // 原始指针
  std::atomic<int>* ref_count;  // 原子引用计数

  // 释放资源（线程安全）
  void release() {
    if (ref_count) {
      // 原子递减并获取递减后的值
      int old_count = ref_count->fetch_sub(1, std::memory_order_acq_rel);
      if (old_count == 1) {
        delete ptr;
        delete ref_count;
        ptr = nullptr;
        ref_count = nullptr;
      }
    }
  }

 public:
  // 默认构造函数
  SharedPtr() : ptr(nullptr), ref_count(nullptr) {}

  // 构造函数，接受原始指针
  explicit SharedPtr(T* p) : ptr(p), ref_count(new std::atomic<int>(1)) {}

  // 拷贝构造函数（线程安全）
  SharedPtr(const SharedPtr<T>& other)
      : ptr(other.ptr), ref_count(other.ref_count) {
    if (ref_count) {
      ref_count->fetch_add(1, std::memory_order_relaxed);
    }
  }

  // 移动构造函数
  SharedPtr(SharedPtr<T>&& other) noexcept
      : ptr(other.ptr), ref_count(other.ref_count) {
    other.ptr = nullptr;
    other.ref_count = nullptr;
  }

  // 析构函数（线程安全）
  ~SharedPtr() { release(); }

  // 拷贝赋值运算符（线程安全）
  SharedPtr<T>& operator=(const SharedPtr<T>& other) {
    if (this != &other) {
      release();
      ptr = other.ptr;
      ref_count = other.ref_count;
      if (ref_count) {
        ref_count->fetch_add(1, std::memory_order_relaxed);
      }
    }
    return *this;
  }

  // 移动赋值运算符
  SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept {
    if (this != &other) {
      release();
      ptr = other.ptr;
      ref_count = other.ref_count;
      other.ptr = nullptr;
      other.ref_count = nullptr;
    }
    return *this;
  }

  // 解引用运算符
  T& operator*() const { return *ptr; }

  // 箭头运算符
  T* operator->() const { return ptr; }

  // 获取原始指针
  T* get() const { return ptr; }

  // 获取引用计数（线程安全）
  int use_count() const {
    return ref_count ? ref_count->load(std::memory_order_acquire) : 0;
  }

  // 检查是否唯一所有者（线程安全）
  bool unique() const { return use_count() == 1; }

  // 重置指针（线程安全）
  void reset(T* p = nullptr) {
    release();
    if (p) {
      ptr = p;
      ref_count = new std::atomic<int>(1);
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

  return 0;
}