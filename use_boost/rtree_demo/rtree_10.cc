#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 2, bg::cs::cartesian> Point;
typedef bg::model::box<Point> Box;         // Define a box type
typedef std::pair<Point, unsigned> Value;  // Pair of point and ID

int main() {
  // Create a vector of points with IDs
  std::vector<Value> points;
  points.push_back(std::make_pair(Point(1.0, 1.0), 1));
  points.push_back(std::make_pair(Point(2.0, 2.0), 2));
  points.push_back(std::make_pair(Point(3.0, 3.0), 3));
  points.push_back(std::make_pair(Point(4.0, 4.0), 4));

  // Construct the R-tree using the packing algorithm
  bgi::rtree<Value, bgi::linear<16>> rtree(points.begin(), points.end());

  // Define a bounding box for the query
  Box query_box(Point(2.5, 2.5),
                Point(3.5, 3.5));  // Lower-left and upper-right corners

  // Perform a spatial query (find points within the bounding box)
  std::vector<Value> result;
  rtree.query(bgi::within(query_box), std::back_inserter(result));

  // Print the results
  std::cout << "Points within the bounding box:" << std::endl;
  for (const auto& val : result) {
    std::cout << "ID: " << val.second << " at (" << bg::get<0>(val.first)
              << ", " << bg::get<1>(val.first) << ")" << std::endl;
  }

  return 0;
}
