// duration constructor
#include <chrono>
#include <cstdint>
#include <iostream>
#include <ratio>
#include <thread>

#include "absl/time/clock.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"

void init_log() {
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 0;

  // Initialize Google's logging library.
  google::InitGoogleLogging("glog_hello");
  google::InstallFailureSignalHandler();  // print backtrace info when crash
}

int64_t TimeNowMicros() {
  // relative time only, cannot used as absolutely
  auto now = std::chrono::steady_clock::now();
  auto time_point_microsecond =
      std::chrono::time_point_cast<std::chrono::microseconds>(now);
  auto epoch = time_point_microsecond.time_since_epoch();
  auto now_microsecond =
      std::chrono::duration_cast<std::chrono::microseconds>(epoch).count();
  return now_microsecond;
}

int64_t TimeNowMicros2() {
  // absolute time
  auto system_now = std::chrono::system_clock::now();
  auto time_point_microsecond =
      std::chrono::time_point_cast<std::chrono::microseconds>(system_now);
  auto epoch = time_point_microsecond.time_since_epoch();
  auto now_microsecond =
      std::chrono::duration_cast<std::chrono::microseconds>(epoch).count();
  return now_microsecond;
}

int64_t AbslTimeNow() { return absl::ToUnixMicros(absl::Now()); }

int main() {
  init_log();
  auto start = TimeNowMicros();
  auto start1 = AbslTimeNow();
  auto start2 = TimeNowMicros2();

  std::this_thread::sleep_for(std::chrono::milliseconds(1'000));

  auto end = TimeNowMicros();
  auto end1 = AbslTimeNow();
  auto end2 = TimeNowMicros2();
  LOG(INFO) << "start: " << start << " \t end: " << end
            << " \t duration(μs): " << end - start;
  LOG(INFO) << "start: " << start1 << " \t end: " << end1
            << " \t duration(μs): " << end1 - start1;
  LOG(INFO) << "start: " << start2 << " \t end: " << end2
            << " \t duration(μs): " << end2 - start2;

  return 0;
}
