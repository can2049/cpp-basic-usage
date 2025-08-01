#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>

int main() {
  using point_t = boost::geometry::model::d2::point_xy<double>;
  using segment_t = boost::geometry::model::segment<point_t>;
  using linestring_t = boost::geometry::model::linestring<point_t>;
  using multipoint_t = boost::geometry::model::multi_point<point_t>;

  segment_t const s{{0, 0}, {2, 2}};
  linestring_t const l{{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 2}};
  point_t p;
  multipoint_t mp;

  std::cout << "point interpolation" << std::endl;

  boost::geometry::line_interpolate(s, std::sqrt(2) / 4, p);
  std::cout << "on segment : " << boost::geometry::wkt(p) << std::endl;

  boost::geometry::line_interpolate(l, 1.4, p);
  std::cout << "on linestring : " << boost::geometry::wkt(p) << std::endl
            << std::endl;

  std::cout << "multipoint interpolation" << std::endl;

  boost::geometry::line_interpolate(s, std::sqrt(2) / 4, mp);
  std::cout << "on segment : " << boost::geometry::wkt(mp) << std::endl;

  mp = multipoint_t();
  boost::geometry::line_interpolate(l, 1.4, mp);
  std::cout << "on linestring : " << boost::geometry::wkt(mp) << std::endl;

  return 0;
}
