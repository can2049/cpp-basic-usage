// https://live.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_examples/specializing_index__indexable_function_object___storing_shared_pointers_in_the_rtree.html
#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace boost {
namespace geometry {
namespace index {

template <typename Box>
struct indexable<boost::shared_ptr<Box> > {
  typedef boost::shared_ptr<Box> V;

  typedef Box const& result_type;
  result_type operator()(V const& v) const { return *v; }
};

}  // namespace index
}  // namespace geometry
}  // namespace boost

int main() {
  typedef bg::model::point<float, 2, bg::cs::cartesian> point;
  typedef bg::model::box<point> box;
  typedef boost::shared_ptr<box> shp;
  typedef shp value;

  // create the rtree using default constructor
  bgi::rtree<value, bgi::linear<16, 4> > rtree;

  std::cout << "filling index with boxes shared pointers:" << std::endl;

  // fill the spatial index
  for (unsigned i = 0; i < 10; ++i) {
    // create a box
    shp b(new box(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f)));

    // display new box
    std::cout << bg::wkt<box>(*b) << std::endl;

    // insert new value
    rtree.insert(b);
  }

  // find values intersecting some area defined by a box
  box query_box(point(0, 0), point(5, 5));
  std::vector<value> result_s;
  rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));

  // find 5 nearest values to a point
  std::vector<value> result_n;
  rtree.query(bgi::nearest(point(0, 0), 5), std::back_inserter(result_n));

  // note: in Boost.Geometry the WKT representation of a box is polygon

  // display results
  std::cout << "spatial query box:" << std::endl;
  std::cout << bg::wkt<box>(query_box) << std::endl;
  std::cout << "spatial query result:" << std::endl;
  BOOST_FOREACH (value const& v, result_s)
    std::cout << bg::wkt<box>(*v) << std::endl;

  std::cout << "knn query point:" << std::endl;
  std::cout << bg::wkt<point>(point(0, 0)) << std::endl;
  std::cout << "knn query result:" << std::endl;
  BOOST_FOREACH (value const& v, result_n)
    std::cout << bg::wkt<box>(*v) << std::endl;

  return 0;
}
