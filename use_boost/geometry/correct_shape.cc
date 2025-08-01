#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <iostream>

int main() {
  using point_t = boost::geometry::model::d2::point_xy<int>;
  typedef boost::geometry::model::polygon<point_t> clockwise_closed_polygon;

  clockwise_closed_polygon cwcp;

  // Fill it counterclockwise (so wrongly), forgetting the closing point
  boost::geometry::exterior_ring(cwcp).push_back(point_t(0, 0));
  boost::geometry::exterior_ring(cwcp).push_back(point_t(10, 10));
  boost::geometry::exterior_ring(cwcp).push_back(point_t(0, 9));

  // Add a counterclockwise closed inner ring (this is correct)
  clockwise_closed_polygon::ring_type inner_ring;
  inner_ring.push_back(point_t(1, 2));
  inner_ring.push_back(point_t(4, 6));
  inner_ring.push_back(point_t(2, 8));
  inner_ring.push_back(point_t(1, 2));
  boost::geometry::interior_rings(cwcp).push_back(inner_ring);

  // Its area should be negative (because of wrong orientation)
  //     and wrong (because of omitted closing point)
  double area_before = boost::geometry::area(cwcp);

  // Correct it!
  boost::geometry::correct(cwcp);

  // Check its new area
  double area_after = boost::geometry::area(cwcp);

  // And output it
  std::cout << boost::geometry::dsv(cwcp) << std::endl;
  std::cout << " area change: " << area_before << " -> " << area_after
            << std::endl;

  return 0;
}
