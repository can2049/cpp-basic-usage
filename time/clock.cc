#include <chrono>
#include <iostream>

int64_t TimeNowMicros_steady() {
  // absolute time
  auto system_now = std::chrono::steady_clock::now();
  auto time_point_microsecond =
      std::chrono::time_point_cast<std::chrono::microseconds>(system_now);
  auto epoch = time_point_microsecond.time_since_epoch();
  auto now_microsecond =
      std::chrono::duration_cast<std::chrono::microseconds>(epoch).count();
  return now_microsecond;
}

int64_t TimeNowMicros_system() {
  // absolute time
  auto system_now = std::chrono::system_clock::now();
  auto time_point_microsecond =
      std::chrono::time_point_cast<std::chrono::microseconds>(system_now);
  auto epoch = time_point_microsecond.time_since_epoch();
  auto now_microsecond =
      std::chrono::duration_cast<std::chrono::microseconds>(epoch).count();
  return now_microsecond;
}

using namespace std;
int main() {
  auto t1 = TimeNowMicros_steady();
  auto t2 = TimeNowMicros_system();
  cout << "t1: " << t1 << endl;
  cout << "t2: " << t2 << endl;
}
