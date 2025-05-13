#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

double hex2double(uint64_t bits) {
  // Reinterpret the binary data as a double
  return *reinterpret_cast<double*>(&bits);
}

uint64_t double2hex(double num) {
  // Reinterpret the binary data as a double
  return *reinterpret_cast<uint64_t*>(&num);
}

int main() {
  std::vector<uint64_t> original_data = {
      0x0,
      0x3f94da11e636cadd,
      0x3f64bd3805122b4d,
      0x3fa3630314cc676d,
      0x3f84b1ea9383dd9e,
      0x3f73827b2f9ca7f3,
      0x3fb0bd1f68757eb4,
      0x3fcbe6e707a1f5ca,
      0x4001db0288858f3a,
      0x3fcea40febf10ab8,
      0x3fcdd1ab6a0c4142,
      0x3fa908cf3d7144fb,
      0x3f9903dfc1a1ee3b,
      0x3f95d9c89c2ec201,
      0x3f711f67a658492c,
      0x3fa023c25dab9620,
      0x3f84595d164d571d,
      0x0,
      0x3f6de8522f18add2,
      0x3f90765d9d85856b,
      0x3f4e337338a59241,
      0x3e50000000000000,
      0x3f909692cf7bae55,
      0x3f62ea0a5d21111b,
      0x0,
      0x3f86df31c9ead4b1,
      0x3f7d10ba2562442c,
      0x3f300fe09d711ace,
      0x3f81ccccdc20b344,
      0x3f75f91963d8081b,
      0x3f5e5a0662ace611,
      0x3f74b695d30dc3d0,
      0x3f6b597afc470e6b,
      0x3f6867fe414502df,
      0x3f8e29a7ca1afd53,
      0x3f9870d5769b0321,
      0x3f82113e8913ddca,
      0x3f711cedd2b3fd28,
      0x3f3992db17398ed4,
      0x3f7c37d370d2028f,
      0x3f40b70336ca1ed1,
      0x3f7eecdadc2f519e,
      0x3f4e322be0c958f2,
      0x3f2b830621ebe51e,
      0x3f520ff7c9cd5d2b,
      0x3f70e1589850a437,
      0x3f5189808d3460ad,
      0x0,
  };

  std::vector<double> data;
  for (auto hex : original_data) {
    auto data_d = hex2double(hex);
    data.push_back(data_d);
    std::cout << std::hex << hex << " -> " << data_d << " -> "
              << double2hex(data_d) << "\n";
  }

  std::cout << "=== begin to sort \n";
  std::sort(data.begin(), data.end(), [](auto a, auto b) { return a <= b; });
  std::cout << "=== finish sort \n";
}
