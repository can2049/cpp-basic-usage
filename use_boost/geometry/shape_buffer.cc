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

  // Create SVG file for visualization
  std::ofstream svg("buffer_visualization.svg");
  boost::geometry::svg_mapper<point> mapper(svg, 800, 600);

  // 1. Linestring buffer
  boost::geometry::model::linestring<point> ls;
  boost::geometry::read_wkt("LINESTRING(0 0,4 5,7 4,10 6)", ls);

  boost::geometry::model::multi_polygon<polygon> ls_result;
  boost::geometry::buffer(ls, ls_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== LINESTRING BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(ls) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(ls_result) << std::endl
            << std::endl;

  // Add to map
  mapper.add(ls);
  mapper.add(ls_result);
  mapper.map(ls, "stroke:blue;stroke-width:3");
  mapper.map(ls_result,
             "fill-opacity:0.3;fill:lightblue;stroke:blue;stroke-width:1");

  // 2. Multi point buffer
  boost::geometry::model::multi_point<point> mp;
  boost::geometry::read_wkt("MULTIPOINT((3 3),(4 4),(6 2))", mp);

  boost::geometry::model::multi_polygon<polygon> mp_result;
  boost::geometry::buffer(mp, mp_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== MULTIPOINT BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(mp) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(mp_result) << std::endl
            << std::endl;

  // Add to map
  mapper.add(mp);
  mapper.add(mp_result);
  mapper.map(mp, "fill:red;stroke:red;stroke-width:2", 5);
  mapper.map(mp_result, "fill-opacity:0.3;fill:pink;stroke:red;stroke-width:1");

  // 3. Multi polygon buffer
  boost::geometry::model::multi_polygon<polygon> mpol;
  boost::geometry::read_wkt(
      "MULTIPOLYGON(((0 1,2 5,5 3,0 1)),((1 1,5 2,5 0,1 1)))", mpol);

  boost::geometry::model::multi_polygon<polygon> mpol_result;
  boost::geometry::buffer(mpol, mpol_result, distance_strategy, side_strategy,
                          join_strategy, end_strategy, circle_strategy);

  std::cout << "=== MULTIPOLYGON BUFFER ===" << std::endl;
  std::cout << "Original: " << boost::geometry::wkt(mpol) << std::endl;
  std::cout << "Buffer:   " << boost::geometry::wkt(mpol_result) << std::endl
            << std::endl;

  // Add to map
  mapper.add(mpol);
  mapper.add(mpol_result);
  mapper.map(mpol, "fill:green;stroke:darkgreen;stroke-width:2");
  mapper.map(mpol_result,
             "fill-opacity:0.3;fill:lightgreen;stroke:green;stroke-width:1");

  std::cout << "Visualization saved to: buffer_visualization.svg" << std::endl;

  return 0;
}
