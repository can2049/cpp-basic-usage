

#include "gflags/gflags.h"
#include "glog/logging.h"
using namespace std;

void init_log() {
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 0;

  // Initialize Google's logging library.
  google::InitGoogleLogging("glog_hello");
  google::InstallFailureSignalHandler();  // print backtrace info when crash
}

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  init_log();
  LOG_IF(INFO, argc > 1) << "this program has parameter";
  SYSLOG_IF(INFO, argc > 1) << "this program has parameter";
  CHECK_GE(argc, 0);
  CHECK(true);
  CHECK(argc);
}
