#include <iostream>
#include <string>
#include <vector>

int main() {
  char in[16] = {0};
  in[0] = 'a';
  in[1] = 'b';
  std::string str1(in, std::size(in));
  std::cout << " std::string1: " << str1 << " size: " << str1.size()
            << std::endl;
  std::string str2(in);
  std::cout << " std::string2: " << str2 << " size: " << str2.size()
            << std::endl;
  return 0;
}
