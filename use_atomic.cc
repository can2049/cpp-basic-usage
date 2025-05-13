#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "glog/logging.h"
#include "glog/stl_logging.h"

std::atomic<uint64_t> acnt{};
uint64_t cnt{};

void count() {
  for (int n = 0; n < 10000; ++n) {
    ++acnt;
    ++cnt;
    // Note: for this example, relaxed memory order
    // is sufficient, e.g. acnt.fetch_add(1, std::memory_order_relaxed);
  }
}

int main() {
  google::InstallFailureSignalHandler();  // print backtrace info when crash
  {
    std::vector<std::thread> pool;
    for (int n = 0; n < 10; ++n) {
      pool.emplace_back(count);
    }
    for (auto& t : pool) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  std::cout << "The atomic counter is " << acnt << '\n'
            << "The non-atomic counter is " << cnt << '\n';
}
