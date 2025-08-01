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
  const double buffer_distance = 1.5;
  const int points_per_circle = 36;
  boost::geometry::strategy::buffer::distance_symmetric<coordinate_type>
      distance_strategy(buffer_distance);
  boost::geometry::strategy::buffer::join_round join_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
  boost::geometry::strategy::buffer::point_circle circle_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::side_straight side_strategy;

  // Create SVG file for multipoint
  std::ofstream svg("multipoint_buffer.svg");
  boost::geometry::svg_mapper<point> mapper(svg, 800, 600);

  // Multi point buffer
  boost::geometry::model::multi_point<point> mp;
  boost::geometry::read_wkt("MULTIPOINT((3 3),(7 5),(5 1),(9 3))", mp);

  boost::geometry::model::multi_polygon<polygon> mp_result;
  boost::geometry::buffer(mp, mp_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== MULTIPOINT BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(mp) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(mp_result) << std::endl;

  // Add to map
  mapper.add(mp);
  mapper.add(mp_result);
  mapper.map(mp, "fill:red;stroke:darkred;stroke-width:3", 8);
  mapper.map(mp_result, "fill-opacity:0.4;fill:pink;stroke:red;stroke-width:2");

  std::cout << "Multipoint visualization saved to: multipoint_buffer.svg"
            << std::endl;

  return 0;
}
