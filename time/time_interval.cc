

#include <glog/logging.h>

#include <chrono>
#include <cstdint>

#include "log.h"

std::int64_t TimeNowMicros() {
  // absolute time
  auto system_now = std::chrono::steady_clock::now();
  auto time_point_microsecond =
      std::chrono::time_point_cast<std::chrono::microseconds>(system_now);
  auto epoch = time_point_microsecond.time_since_epoch();
  auto now_microsecond =
      std::chrono::duration_cast<std::chrono::microseconds>(epoch).count();
  return now_microsecond;
}

bool TimeIntervalReady(std::int64_t time_interval_in_us) {
  // do calibration every interval
  static int64_t last_calibration_time_point = TimeNowMicros();
  int64_t time_point_now = TimeNowMicros();
  if (time_point_now - last_calibration_time_point < time_interval_in_us) {
    return false;
  }
  last_calibration_time_point = time_point_now;
  return true;
}

int main() {
  while (true) {
    if (TimeIntervalReady(1000 * 1000)) {
      LOG(INFO) << "hi";
    }
  }
}
