#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/types/span.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
using namespace std;

#include "execinfo.h"

std::string ExecCmd(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  result.reserve(512);
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void FailureWriter(const char* data, int size) {
  // run only once
  [[maybe_unused]] const static bool backtrace_printed = []() {
    LOG(ERROR) << "Start analyzing backtrace...";
    constexpr size_t kMaxTraceDepth = 100;
    void* buffer[kMaxTraceDepth];
    memset(buffer, 0, sizeof(buffer));

    int depth = backtrace(buffer, kMaxTraceDepth);
    char** symbols = backtrace_symbols(buffer, kMaxTraceDepth);
    const auto pid = getpid();

    for (int i = 1; i < depth; ++i) {
      constexpr size_t kMaxCmdLength = 256;
      char get_line_number_cmd[kMaxCmdLength];
      snprintf(get_line_number_cmd, kMaxCmdLength, "eu-addr2line '%p' --pid=%d",
               buffer[i], pid);
      LOG(ERROR) << "@\t" << symbols[i] << ExecCmd(get_line_number_cmd);
    }
    google::FlushLogFiles(0);
    return true;
  }();

  std::string_view str_brief(data, size);
  LOG(ERROR) << str_brief;
  google::FlushLogFiles(0);
}

void init_log() {
  // if (!google::IsGoogleLoggingInitialized()) {
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 0;

  // Initialize Google's logging library.
  google::InitGoogleLogging("glog_hello");
  google::InstallFailureSignalHandler();  // print backtrace info when crash
  google::InstallFailureWriter(FailureWriter);
}

void crash() {
  vector<int> data{1, 2, 3};
  auto a = data.at(10);
  LOG(INFO) << " a: " << a;
  int* p = nullptr;
  *p = 1;
}

int main(int argc, char* argv[]) {
  init_log();

  // Optional: parse command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  LOG(ERROR) << "AAAAA";
  LOG(WARNING) << "AAAAAA";
  LOG(INFO) << "AAAAAA";
  // uint32_t smt_value = 8;
  // long shift = -1;
  // smt_value >>= shift;
  // LOG(INFO) << smt_value;

  crash();

  cout << "==============" << endl;
}
