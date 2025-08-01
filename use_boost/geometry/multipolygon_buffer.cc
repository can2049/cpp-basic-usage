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
  const double buffer_distance = 0.8;
  const int points_per_circle = 36;
  boost::geometry::strategy::buffer::distance_symmetric<coordinate_type>
      distance_strategy(buffer_distance);
  boost::geometry::strategy::buffer::join_round join_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
  boost::geometry::strategy::buffer::point_circle circle_strategy(
      points_per_circle);
  boost::geometry::strategy::buffer::side_straight side_strategy;

  // Create SVG file for multipolygon
  std::ofstream svg("multipolygon_buffer.svg");
  boost::geometry::svg_mapper<point> mapper(svg, 800, 600);

  // Multi polygon buffer
  boost::geometry::model::multi_polygon<polygon> mpol;
  boost::geometry::read_wkt(
      "MULTIPOLYGON(((1 1,3 6,6 4,1 1)),((2 0,6 1,6 -1,2 0)),((7 2,9 4,11 1,7 "
      "2)))",
      mpol);

  boost::geometry::model::multi_polygon<polygon> mpol_result;
  boost::geometry::buffer(mpol, mpol_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== MULTIPOLYGON BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(mpol) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(mpol_result) << std::endl;

  // Add to map
  mapper.add(mpol);
  mapper.add(mpol_result);
  mapper.map(mpol, "fill:green;stroke:darkgreen;stroke-width:3");
  mapper.map(mpol_result,
             "fill-opacity:0.4;fill:lightgreen;stroke:green;stroke-width:2");

  std::cout << "Multipolygon visualization saved to: multipolygon_buffer.svg"
            << std::endl;

  return 0;
}
