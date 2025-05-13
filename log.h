#pragma once

#include "gflags/gflags.h"
#include "glog/logging.h"
using namespace std;

inline void init_log() {
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 0;

  // Initialize Google's logging library.
  google::InitGoogleLogging("glog_hello");
  google::InstallFailureSignalHandler();  // print backtrace info when crash
}
