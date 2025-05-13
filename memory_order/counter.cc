#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> counter(0);

void worker(int n) {
  int local_counter = 0;
  for (int i = 0; i < n; ++i) {
    // do other operation
    local_counter += 1;
  }
  // atomic operation in the final
  counter.fetch_add(local_counter, std::memory_order_seq_cst);
}

int main() {
  const int n = 100000;
  const int num_threads = 88;
  std::vector<std::thread> threads;
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(worker, n);
  }
  for (auto& t : threads) {
    t.join();
  }
  std::cout << counter << std::endl;
  assert(n * num_threads == counter);
  return 0;
}
