
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<double> point_type;
typedef bg::model::linestring<point_type> linestring_type;

double point_distance(const point_type& p1, const point_type& p2) {
  return bg::distance(p1, p2);
}

int main() {
  linestring_type line1, line2;

  for (int i = -5; i < 6; ++i) {
    double y1 = i * i * i;
    double y2 = y1 + 2.0;
    bg::append(line1, point_type(i, y1));
    bg::append(line2, point_type(i, y2));
  }

  // 计算离散弗雷歇距离
  double frechet_distance = bg::discrete_frechet_distance(line1, line2);
  std::cout << "Discrete Fréchet Distance: " << frechet_distance << std::endl;

  double line_distance = bg::distance(line1, line2);
  std::cout << "Line Distance: " << line_distance << std::endl;


  double dist = std::numeric_limits<double>::max();
  for(auto& p1: line1){
    for (auto & p2: line2) {
      double dist2 = point_distance(p1, p2);
      dist = std::min(dist, dist2);
    }
  }
  std::cout << "Minimum Point Distance: " << dist << std::endl;

  return 0;
}
