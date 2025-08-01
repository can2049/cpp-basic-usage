#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <fstream>
#include <iostream>

int main() {
  typedef double coordinate_type;
  typedef boost::geometry::model::d2::point_xy<coordinate_type> point;
  typedef boost::geometry::model::polygon<point> polygon;

  // Declare strategies
  const double buffer_distance = 1.0;
  const int points_per_circle = 36;
  boost::geometry::strategy::buffer::distance_symmetric<coordinate_type>
      distance_strategy(buffer_distance);
  boost::geometry::strategy::buffer::join_round join_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
  boost::geometry::strategy::buffer::point_circle circle_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::side_straight side_strategy;

  // Create SVG file for linestring
  std::ofstream svg("linestring_buffer.svg");
  boost::geometry::svg_mapper<point> mapper(svg, 800, 600);

  // Linestring buffer
  boost::geometry::model::linestring<point> ls;
  boost::geometry::read_wkt("LINESTRING(2 2,6 7,9 6,12 8)", ls);

  boost::geometry::model::multi_polygon<polygon> ls_result;
  boost::geometry::buffer(ls, ls_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== LINESTRING BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(ls) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(ls_result) << std::endl;

  // Add to map
  mapper.add(ls);
  mapper.add(ls_result);
  mapper.map(ls, "stroke:blue;stroke-width:4");
  mapper.map(ls_result,
             "fill-opacity:0.4;fill:lightblue;stroke:darkblue;stroke-width:2");

  std::cout << "Linestring visualization saved to: linestring_buffer.svg"
            << std::endl;

  return 0;
}
