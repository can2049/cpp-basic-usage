#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <array>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

int get_cpu_id() {
  /* Get the the current process' stat file from the proc filesystem */
  FILE* procfile = fopen("/proc/self/stat", "r");
  long to_read = 8192;
  char buffer[to_read];
  auto read = fread(buffer, sizeof(char), to_read, procfile);
  (void)read;
  fclose(procfile);

  // Field with index 38 (zero-based counting) is the one we want
  char* line = strtok(buffer, " ");
  for (int i = 1; i < 38; i++) {
    line = strtok(NULL, " ");
  }

  line = strtok(NULL, " ");
  int cpu_id = atoi(line);
  return cpu_id;
}

void thread_info() {
  auto tid = this_thread::get_id();
  auto cpu_id = get_cpu_id();
  //   cout << "| this thread id: " << id << " cpu: {" << cpu_id << "} " <<
  //   endl;
  cout << "[" << cpu_id << "]"
       << " tid: " << tid << endl
       << flush;
}

int main2() {
  thread_info();
  cout << "===================" << endl;
  vector<thread> ts;
  for (auto i = 0; i < 50; i++) {
    ts.push_back(thread(thread_info));
  }
  for (auto& t : ts) {
    if (t.joinable()) {
      t.join();
    }
  }
  return 0;
}

int main() {
  constexpr unsigned num_threads = 4;
  // A mutex ensures orderly access to std::cout from multiple threads.
  std::mutex iomutex;
  std::vector<std::thread> threads(num_threads);
  for (unsigned i = 0; i < num_threads; ++i) {
    threads[i] = std::thread([&iomutex, i] {
      while (1) {
        {
          // Use a lexical scope and lock_guard to safely lock the mutex only
          // for the duration of std::cout usage.
          std::lock_guard<std::mutex> iolock(iomutex);
          std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
        }

        // Simulate important work done by the tread by sleeping for a bit...
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }
  return 0;
}
