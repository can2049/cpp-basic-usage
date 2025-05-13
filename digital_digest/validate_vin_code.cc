#include <glog/logging.h>

#include <cctype>
#include <iostream>
#include <string_view>
#include <unordered_map>

bool IsValidVIN(std::string_view vin) {
  if (vin.length() != 17) {
    return false;
  }

  std::unordered_map<char, int> weight_map = {
      {'A', 1}, {'B', 2}, {'C', 3}, {'D', 4}, {'E', 5}, {'F', 6}, {'G', 7},
      {'H', 8}, {'J', 1}, {'K', 2}, {'L', 3}, {'M', 4}, {'N', 5}, {'P', 7},
      {'R', 9}, {'S', 2}, {'T', 3}, {'U', 4}, {'V', 5}, {'W', 6}, {'X', 7},
      {'Y', 8}, {'Z', 9}, {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
      {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}};

  constexpr int position_weight[] = {8, 7, 6, 5, 4, 3, 2, 10, 0,
                                     9, 8, 7, 6, 5, 4, 3, 2};

  int sum = 0;
  for (size_t i = 0; i < vin.length(); ++i) {
    char c = std::toupper(vin[i]);
    if (weight_map.find(c) == weight_map.end()) {
      // 1. use only letters and digits
      // 2. 'I', 'O', 'Q' are not allowed
      return false;
    }
    sum += weight_map[c] * position_weight[i];
  }

  int checkDigit = sum % 11;
  char expectedCheckDigit = (checkDigit == 10) ? 'X' : ('0' + checkDigit);

  return vin[8] == expectedCheckDigit;
}

int main() {
  CHECK(IsValidVIN("L6T794CD4RV000286"));
  CHECK(IsValidVIN("L6T79RND5SV000023"));
  CHECK(IsValidVIN("L6T79RND8SV000016"));
  CHECK(!IsValidVIN("L6T79ICD4RV000286"));   // not valide, contains 'I'
  CHECK(!IsValidVIN("L6T79OCD4RV000286"));   // not valide, contains 'O'
  CHECK(!IsValidVIN("L6T79QCD4RV000286"));   // not valide, contains 'Q'
  CHECK(!IsValidVIN("L6T794CD4RV00028"));    // not valide, length is 16
  CHECK(!IsValidVIN("L6T794CD4RV0002860"));  // not valide, length is 18
  std::cout << " all test passed!\n";
}
