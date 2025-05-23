// https://live.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_examples/iterative_query.html
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

// just for output
#include <iostream>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

int main() {
  typedef bg::model::point<double, 2, bg::cs::cartesian> point;
  typedef point value;
  typedef bgi::rtree<value, bgi::linear<16> > rtree_t;

  // create the rtree using default constructor
  rtree_t rtree;

  // create some values
  for (double f = 0; f < 10; f += 1) {
    // insert new value
    rtree.insert(point(f, f));
  }

  // query point
  point pt(5.1, 5.1);

  // iterate over nearest Values
  for (rtree_t::const_query_iterator it = rtree.qbegin(bgi::nearest(pt, 100));
       it != rtree.qend(); ++it) {
    double d = bg::distance(pt, *it);

    std::cout << bg::wkt(*it) << ", distance= " << d << std::endl;

    // break if the distance is too big
    if (d > 2) {
      std::cout << "break!" << std::endl;
      break;
    }
  }

  return 0;
}
