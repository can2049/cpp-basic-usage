#include <algorithm>
#include <iostream>

#include "absl/strings/str_split.h"

/**
 * @brief compare 2 version
 *
 * @param version_to_cmp dot format, like "1.2.3"
 * @param version_base
 * @return int 0: equal, 1: version newer to base, -1: version older to base
 */
int CompareVersion(const std::string& version_to_cmp,
                   const std::string& version_base) {
  std::vector<std::string> segs1 = absl::StrSplit(version_to_cmp, '.');
  std::vector<std::string> segs0 = absl::StrSplit(version_base, '.');
  auto seg_len = std::min(segs0.size(), segs1.size());
  for (auto i = 0U; i < seg_len; i++) {
    auto v1 = std::stoi(segs1.at(i));
    auto v0 = std::stoi(segs0.at(i));
    if (v1 > v0) {
      return 1;
    }
    if (v1 < v0) {
      return -1;
    }
  }
  if (segs1.size() > segs0.size()) {
    return 1;
  }
  if (segs1.size() < segs0.size()) {
    return -1;
  }
  return 0;
}

using namespace std;
int main() {
  auto v0 = "1.2.3";
  auto v1 = "1.2.5";
  auto v2 = "1.2.2";
  auto v3 = "1.2.3";

  cout << CompareVersion(v1, v0) << endl;
  cout << CompareVersion(v2, v0) << endl;
  cout << CompareVersion(v3, v0) << endl;
}
