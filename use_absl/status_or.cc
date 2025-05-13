#include "absl/status/status.h"
#include "absl/status/statusor.h"
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

absl::StatusOr<int> func(int a) {
  if (a > 0) {
    return absl::InvalidArgumentError("input error");
  }
  return a * a * a;
}

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  init_log();
  auto ret = func(argc);
  LOG(INFO) << ret.status().ToString();
  LOG(INFO) << ret.status();
}
