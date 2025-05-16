#include <iostream>
#include <vector>

#include "data_def.pb.h"
using namespace std;

int main() {
  vector<float> h{-1, -2, 1, 2};
  Data data;
  data.mutable_height()->CopyFrom({h.begin(), h.end()});

  for (auto h : data.height()) {
    cout << h << ", ";
  }
  cout << endl;
}
