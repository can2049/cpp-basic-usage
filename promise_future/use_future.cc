#include <future>
#include <iostream>
#include <thread>

int main() {
  // future from a packaged_task
  std::packaged_task<int()> task([] { return 7; });  // wrap the function
  std::future<int> f1 = task.get_future();           // get a future
  std::thread t(std::move(task));                    // launch on a thread

  // future from an async()
  std::future<int> f2 = std::async(std::launch::async, [] { return 8; });

  // future from a promise
  std::promise<int> p;
  std::future<int> f3 = p.get_future();
  std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

  std::cout << "Waiting..." << std::flush;
  f1.wait();
  f2.wait();
  f3.wait();
  std::cout << "Done!\nResults are: " << f1.get() << ' ' << f2.get() << ' '
            << f3.get() << '\n';
  t.join();
}
