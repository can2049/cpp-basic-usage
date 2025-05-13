#include <iostream>

int main() {
  constexpr std::string_view kModelInputLayers[] = {
      "navi_command_points_norm",
      "navi_command_lane_number",
      "navi_command_out_link_angle_bitmap",
      "navi_command_target_link_angle_bitmap",
      "navi_command_bitmap",
      "navi_command_index",
      "navi_command_padding_mask",
  };
  std::cout << " size: " << sizeof(kModelInputLayers) << std::endl;
}
