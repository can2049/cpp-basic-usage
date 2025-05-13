#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
int32_t GenerateMapId(const std::string& car_id) {
  auto current_time = std::time(nullptr);
  std::string input_str = car_id + std::to_string(current_time);
  size_t new_hash = std::hash<std::string>{}(input_str);
  // keep lower 31 bits
  constexpr size_t kMapIdMask = (1U << 31) - 1;
  size_t final_hash = new_hash & kMapIdMask;
  return static_cast<int32_t>(final_hash);
}

int main() {
  std::string car_id = "car123";
  int32_t map_id = GenerateMapId(car_id);
  // print in hex formatting, 32 bits width
  std::cout << "Generated Map ID: " << std::hex << std::setw(8)
            << std::setfill('0') << map_id << std::endl;

  return 0;
}
