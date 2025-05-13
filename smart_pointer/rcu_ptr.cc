
// https://martong.github.io/high-level-cpp-rcu_informatics_2017.pdf

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

template <typename T>
class rcu_ptr {
  // source pointer
  std::shared_ptr<const T> sp;

 public:
  rcu_ptr() = default;
  ~rcu_ptr() = default;

  rcu_ptr(const rcu_ptr &rhs) = delete;
  rcu_ptr &operator=(const rcu_ptr &rhs) = delete;
  rcu_ptr(rcu_ptr &&) = delete;
  rcu_ptr &operator=(rcu_ptr &&) = delete;

  rcu_ptr(const std::shared_ptr<const T> &sp_) : sp(sp_) {}
  rcu_ptr(std::shared_ptr<const T> &&sp_) : sp(std::move(sp_)) {}

  std::shared_ptr<const T> read() const {
    return std::atomic_load_explicit(&sp, std::memory_order_consume);
  }

  void reset(std::shared_ptr<const T> &&r) {
    std::atomic_store_explicit(&sp, std::forward(r), std::memory_order_release);
  }

  template <typename R>
  void copy_update(R &&fun) {
    std::shared_ptr<const T> sp_local =
        std::atomic_load_explicit(&sp, std::memory_order_consume);

    std::shared_ptr<T> replace;
    do {
      if (sp_local) {
        // deep copy
        replace = std::make_shared<T>(*sp_local);
      }

      // update
      std::forward<R>(fun)(replace.get());

    } while (!std::atomic_compare_exchange_strong_explicit(
        &sp, &sp_local, std::shared_ptr<const T>(std::move(replace)),
        std::memory_order_release, std::memory_order_consume));
  }
};

class X {
  rcu_ptr<std::vector<int>> v;

 public:
  X() : v(std::make_shared<std::vector<int>>()) {}

  int sum() const {  // read operation
    std::shared_ptr<const std::vector<int>> local_copy = v.read();
    return std::accumulate(local_copy->begin(), local_copy->end(), 0);
  }

  void add(int i) {  // write operation
    v.copy_update([i](std::vector<int> *copy) { copy->push_back(i); });
  }

  void remove_last() {
    v.copy_update([](std::vector<int> *copy) { copy->pop_back(); });
  }
};

int main() {
  X x;
  x.add(1);
  x.add(2);
  x.add(3);
  std::cout << x.sum() << std::endl;
  x.remove_last();
  std::cout << x.sum() << std::endl;
  return 0;
}
