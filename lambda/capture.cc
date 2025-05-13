#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>
#include <vector>

class DataProcessor {
 public:
  void processData() {
    int total = 0;
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << " numbers original size: " << numbers.size() << std::endl;
    std::string status = "Processing";
    auto p = std::make_shared<int>(123);

    // Using & to capture variables by reference
    auto calculator = [&, numbers = numbers](int multiplier) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << " numbers inside lambda size: " << numbers.size()
                << std::endl;
      total += 100;          // Can modify total because of & capture
      status = "Completed";  // Can modify status because of & capture

      // 'numbers' is captured by value (copied) because it's explicitly
      // specified
      for (const auto& num : numbers) {
        std::cout << num * multiplier << " ";
      }
      std::cout << "\n";
      // numbers.push_back(10);
    };

    numbers.push_back(6);

    calculator(2);
    std::cout << " numbers after lambda size: " << numbers.size() << std::endl;
    std::cout << "\nTotal: " << total << std::endl;
    std::cout << "Status: " << status << std::endl;
  }
};

int main() {
  DataProcessor processor;
  processor.processData();
  return 0;
}
