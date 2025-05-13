#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"
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

int main() {
  init_log();
  std::string s = "!hello, world!!!";
  auto s2 = absl::StripSuffix(s, "!");
  auto s3 = absl::StripPrefix(s2, "!");
  LOG(INFO) << s << " -> " << s2;
  LOG(INFO) << s2 << " -> " << s3;
  return 0;
}
