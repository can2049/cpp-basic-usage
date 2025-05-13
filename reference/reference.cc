#include <iostream>
#include <memory>
#include <thread>
#include <vector>

int main() {
  std::vector<std::thread> threads;

  {
    auto ptr = std::make_shared<int>(10);

    std::thread t1([&, _ptr = ptr]() {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::cout << "t1 ptr valid? " << (ptr != nullptr) << std::endl;
      std::cout << "t1 _ptr valid? " << (_ptr != nullptr) << std::endl;
      std::cout << "t1 ptr: " << ptr << std::endl;
      std::cout << "t1 _ptr: " << _ptr << std::endl;
      std::cout << "t1 *_ptr: " << *_ptr << std::endl;
      std::cout << "t1 *ptr: " << *ptr << std::endl;
      std::cout << "t1 ptr count: " << ptr.use_count() << std::endl;
      std::cout << "t1 _ptr count: " << _ptr.use_count() << std::endl;
    });

    threads.push_back(std::move(t1));

    std::thread t2([ptr]() {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "t2 ptr valid? " << (ptr != nullptr) << std::endl;
      std::cout << "t2 ptr: " << ptr << std::endl;
      std::cout << "t2 *ptr: " << *ptr << std::endl;
    });

    threads.push_back(std::move(t2));

    std::cout << " exit scope \n";
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
  return 0;
}
