#include <chrono>
#include <ctime>
#include <iostream>

int main() {
  auto now = std::chrono::system_clock::now();
  std::cout << __FILE__ << ":" << __LINE__ << " "
            << now.time_since_epoch().count() << "\n";
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::cout << __FILE__ << ":" << __LINE__ << " "
            << " Current time: " << std::ctime(&now_time);

  std::time_t result = std::time(nullptr);
  std::cout << __FILE__ << ":" << __LINE__ << " " << result << "\n";
  std::cout << __FILE__ << ":" << __LINE__ << " "
            << std::asctime(std::localtime(&result)) << result
            << " seconds since the Epoch\n";
  std::cout << __FILE__ << ":" << __LINE__ << " Time retrieval complete.\n";
}
