#include <iostream>
int main() {
  constexpr int SD_X_MIN = -60;
  constexpr int SD_X_MAX = 140;
  constexpr int SD_Y_MIN = -60;
  constexpr int SD_Y_MAX = 60;

  int img_width = 240;
  int img_height = 308;

  int ego_u = img_width * (SD_Y_MAX / (1.0 * SD_Y_MAX - SD_Y_MIN));
  int ego_v = img_height * (SD_X_MAX / (1.0 * SD_X_MAX - SD_X_MIN));

  std::cout << "ego_u: " << ego_u << " ego_v: " << ego_v << std::endl;
}
