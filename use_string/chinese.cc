#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

int main() {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  std::string utf8_str = "你好，世界！";
  std::u32string utf32_str = conv.from_bytes(utf8_str);
  std::cout << "字符数: " << utf32_str.size() << std::endl;
  std::cout << "char size: " << utf8_str.size() << std::endl;
}
