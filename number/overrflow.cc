#include <algorithm>
#include <iostream>

int main(int argc, char** argv) {
  float pixel_value = std::stof(argv[1]);
  int pixel = pixel_value * 255 + 0.5;
  uint8_t pixel_uint8 = std::clamp(pixel, 0, 255);
  std::cout << (int)pixel_uint8 << std::endl;
}
