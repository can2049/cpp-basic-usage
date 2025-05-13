#include <iostream>
#include <string_view>

#include "absl/strings/match.h"
#include "absl/strings/str_cat.h"

std::string SimpleJson(absl::string_view key, absl::string_view value) {
  return absl::StrCat("{\"", key, "\": \"", value, "\"}");
}

std::string SimpleJson2(std::string_view key, std::string_view value) {
  std::string ans;
  ans.reserve(key.size() + value.size() + 4);
  ans += "{\"";
  ans += key;
  ans += "\": \"";
  ans += value;
  ans += "\"}";
  return ans;
}

using namespace std;

int main() {
  auto a = SimpleJson("a", "b");
  std::cout << a << std::endl;

  if (absl::StrContains("aaa", "a")) {
  }

  auto bb = SimpleJson2("name", "bbbbbob");
  cout << bb << endl;

  string s1("abcdddddddddddddddddddddddddddddddddddd");
  s1.shrink_to_fit();
  absl::string_view sv = "aaaaa";
  sv.npos;
  cout << "s1. capacity: " << s1.capacity() << " size: " << s1.size() << endl;
}
