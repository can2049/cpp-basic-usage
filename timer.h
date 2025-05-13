

#pragma once
#include <chrono>

/**
 * @brief A high precision, fast and robust Timer
 *
 */
class Timer {
 public:
  Timer(const Timer &) = delete;
  Timer &operator=(const Timer &) = delete;
  Timer(Timer &&) = delete;
  Timer &operator=(Timer &&) = delete;
  Timer() { Start(); }
  /**
   * @brief reset start point.
   *
   */
  void Start() { start_ = std::chrono::steady_clock::now(); }
  /**
   * @brief Count time duration in milliseconds(ms, 1s = 1000ms)
   *
   * @param reset reset start point
   * @return double duration in milliseconds
   */
  double EndMilli(bool reset = false) {
    using std::chrono::duration_cast;
    using std::chrono::microseconds;
    auto now = std::chrono::steady_clock::now();
    auto duration = duration_cast<microseconds>(now - start_).count();
    if (reset) {
      Start();
    }
    return static_cast<double>(duration) / 1000.0;
  }

 private:
  std::chrono::time_point<std::chrono::steady_clock> start_;
};
