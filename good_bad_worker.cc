
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#define THREAD_NUM 8
std::vector<double> tmp_sum(THREAD_NUM, 0);

void BadWorker(int idx, int min, int max) {
  for (int i = min; i <= max; ++i) {
    if (i % 2 == 0) {
      ++tmp_sum[idx];
    }
  }
}

void GoodWorker(int idx, int min, int max) {
  int tmp = 0;
  for (int i = min; i <= max; ++i) {
    if (i % 2 == 0) {
      ++tmp;
    }
  }
  tmp_sum[idx] = tmp;
}

int CountEvenConcurrent(std::function<void(int, int, int)> f, int min,
                        int max) {
  const int thread_num = THREAD_NUM;
  std::vector<std::thread> threads;
  for (int t = 0; t < thread_num; ++t) {
    int range = max / thread_num * (t + 1);
    threads.push_back(std::thread(f, t, min, range));
    min = range + 1;
  }

  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i].join();
  }

  int even = 0;
  for (size_t i = 0; i < threads.size(); ++i) {
    even += tmp_sum[i];
  }

  return even;
}

int main() {
  constexpr int val = 20e8;
  int bad_worker_res = CountEvenConcurrent(GoodWorker, 0, val);
  std::cout << bad_worker_res << "\n";

  int good_worker_res = CountEvenConcurrent(BadWorker, 0, val);
  std::cout << good_worker_res << "\n";

  return 0;
}
