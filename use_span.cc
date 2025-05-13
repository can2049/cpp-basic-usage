#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_join.h"
#include "absl/types/span.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"
using namespace std;

int id{10};

struct NNInternalDataType {
  std::vector<int> nodes;
  int id_{};
  int frame_id{};
  NNInternalDataType() {
    id_ = id;
    id++;
    cout << "nn begin..." << id_ << endl;
  }
  ~NNInternalDataType() { cout << "nn end..." << id_ << endl; }
  NNInternalDataType(NNInternalDataType &t) {
    cout << "nn copy..." << id_ << "->" << t.id_ << endl;
    nodes = t.nodes;
  }
  NNInternalDataType(const NNInternalDataType &t) {
    cout << "nn const copy..." << id_ << "->" << t.id_ << endl;
    nodes = t.nodes;
  }
};

NNInternalDataType MergeNnData(absl::Span<const NNInternalDataType> nn_inputs) {
  NNInternalDataType output;
  output.frame_id = nn_inputs.front().frame_id;
  auto &out_nodes = output.nodes;
  for (const auto &nn : nn_inputs) {
    out_nodes.insert(out_nodes.end(), nn.nodes.begin(), nn.nodes.end());
  }
  return output;
}

int main(int argc, char *argv[]) {
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 0;

  // Initialize Google's logging library.
  google::InitGoogleLogging("glog_hello");
  google::InstallFailureSignalHandler();

  // const absl::Span<const int> data{1, 2, 3, 4, 5};  // dangerous initialize
  const std::vector<int> data2{1, 2, 3, 4, 5};  // GOOD initialize
  // absl::Span<const int> data3(data2.begin(), data2.end());
  absl::Span<const int> data3(data2);
  // LOG(ERROR) << "DATA: " << absl::StrJoin(data, ", ");
  LOG(INFO) << "DATA: " << absl::StrJoin(data2, ", ");
  LOG(INFO) << "DATA: " << absl::StrJoin(data3, ", ");

  // Optional: parse command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  NNInternalDataType a;
  a.nodes = {1, 2, 3};
  NNInternalDataType b;
  b.nodes = {4, 5, 6};
  NNInternalDataType c;

  c.nodes = {7, 8};
  LOG(INFO) << "==============";
  auto d = MergeNnData({a, b, c});
  LOG(INFO) << d.nodes.size();
}
