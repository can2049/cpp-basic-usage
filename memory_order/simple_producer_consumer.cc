#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> data;
std::atomic<bool> ready(false);

void producer() {
  data.store(42, std::memory_order_relaxed);
  ready.store(true, std::memory_order_release);
}

void consumer() {
  while (!ready.load(std::memory_order_acquire))
    ;
  std::cout << data.load(std::memory_order_relaxed) << std::endl;
}

int main() {
  std::thread t1(producer);
  std::thread t2(consumer);
  t1.join();
  t2.join();
  return 0;
}
