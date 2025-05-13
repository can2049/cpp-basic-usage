#include <boost/circular_buffer.hpp>
#include <boost/circular_buffer/base.hpp>

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
  // Create a circular buffer with a capacity for 3 integers.
  boost::circular_buffer<int> cb(3);

  CHECK(cb.empty());

  // Insert threee elements into the buffer.
  cb.push_back(1);
  cb.push_back(2);
  cb.push_back(3);

  CHECK(cb.full());

  CHECK_EQ(cb.front(), 1);

  CHECK_EQ(cb[0], 1);
  CHECK_EQ(cb[1], 2);
  CHECK_EQ(cb[2], 3);

  CHECK_EQ(cb.back(), 3);

  // The buffer is full now, so pushing subsequent
  // elements will overwrite the front-most elements.

  cb.push_back(4);  // Overwrite 1 with 4.
  cb.push_back(5);  // Overwrite 2 with 5.

  // The buffer now contains 3, 4 and 5.
  CHECK_EQ(cb.front(), 3);
  CHECK_EQ(cb[0], 3);
  CHECK_EQ(cb[1], 4);
  CHECK_EQ(cb[2], 5);

  CHECK_EQ(cb.back(), 5);

  // Elements can be popped from either the front or the back.
  cb.pop_back();   // 5 is removed.
  cb.pop_front();  // 3 is removed.

  // Leaving only one element with value = 4.
  CHECK_EQ(cb[0], 4);

  cb.push_front(6);
  CHECK_EQ(cb.front(), 6);

  int a = 88;
  cb.front() = a;
  CHECK_EQ(cb.front(), a);

  LOG(INFO) << "end.";

  LOG(INFO) << " sizeof(boost::circular_buffer<char>) "
            << sizeof(boost::circular_buffer<char>);
  LOG(INFO) << " sizeof(boost::circular_buffer<int>) "
            << sizeof(boost::circular_buffer<int>);
  LOG(INFO) << " sizeof(boost::circular_buffer<float>) "
            << sizeof(boost::circular_buffer<float>);
  return 0;
}
