#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <cmath>
#include <iostream>

int main() {
  using point_type = boost::geometry::model::d2::point_xy<double>;

  point_type p1(0, 0);
  point_type p2(1, 1);

  // Calculate azimuth (bearing) manually using atan2
  double dx = boost::geometry::get<0>(p2) - boost::geometry::get<0>(p1);
  double dy = boost::geometry::get<1>(p2) - boost::geometry::get<1>(p1);
  double azimuth = std::atan2(dy, dx);

  std::cout << "azimuth: " << azimuth << std::endl;

  return 0;
}
