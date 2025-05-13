#include <iostream>
#include <string>
#include <string_view>

int main() {
  const char* str = "hello, world!";
  std::cout << " str: " << str << std::endl;
  std::string_view sv(str);
  std::cout << " sv: " << sv << std::endl;
  std::cout << " sv.size(): " << sv.size() << std::endl;
  std::string_view sub_sv = sv.substr(7, 8);
  std::cout << " sub_sv: " << sub_sv << std::endl;
  std::cout << " sub_sv.size(): " << sub_sv.size() << std::endl;
  std::string str2(sub_sv);
  std::cout << " str2: " << str2 << std::endl;
  std::string str3(sub_sv.data(), sub_sv.size());
  std::cout << " str3: " << str3 << std::endl;
}
