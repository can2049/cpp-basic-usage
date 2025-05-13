#include <iostream>
#include <string_view>

inline bool iequals(std::string_view a, std::string_view b) {
  return std::equal(
      a.begin(), a.end(), b.begin(), b.end(),
      [](char a, char b) -> bool { return tolower(a) == tolower(b); });
}

using namespace std;

int main() {
  string s1 = "Hello";
  string s2 = "hello";

  cout << "s1 == s2: " << (s1 == s2) << endl;
  cout << "s1 != s2: " << (s1 != s2) << endl;
  cout << "s1 <  s2: " << (s1 < s2) << endl;
  cout << "s1 >  s2: " << (s1 > s2) << endl;
  cout << "s1 <= s2: " << (s1 <= s2) << endl;
  cout << "s1 >= s2: " << (s1 >= s2) << endl;
  cout << "s1 ieaual s2? " << iequals(s1, s2) << endl;
}
