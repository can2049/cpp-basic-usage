
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>

struct PointXYZIBT_1 {
  float x, y, z;
  ::std::uint8_t intensity;
  ::std::uint8_t ring;
  uint16_t timestamp;
  // some functions
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
}
// EIGEN_ALIGN16
;

struct PointXYZIBT_2 {
  PCL_ADD_POINT4D;  // This adds the members x,y,z which can also be accessed
                    // using the point (which is float[4])
  ::std::uint8_t intensity;
  ::std::uint8_t ring;
  uint16_t timestamp;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using _custom_allocator_type_trait = void;
}
// EIGEN_ALIGN16
;

int main() {
  std::cout << "our style size: " << sizeof(PointXYZIBT_1) << std::endl;
  std::cout << "pcl style size: " << sizeof(PointXYZIBT_2) << std::endl;
}
