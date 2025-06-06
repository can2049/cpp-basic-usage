// https://live.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_examples/using_indexablegetter_function_object___storing_indexes_of_external_container_s_elements.html

#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

template <typename Container>
class my_indexable {
  typedef typename Container::size_type size_t;
  typedef typename Container::const_reference cref;
  Container const& container;

 public:
  typedef cref result_type;
  explicit my_indexable(Container const& c) : container(c) {}
  result_type operator()(size_t i) const { return container[i]; }
};

int main() {
  typedef bg::model::point<float, 2, bg::cs::cartesian> point;
  typedef bg::model::box<point> box;
  typedef std::vector<box>::size_type value;
  typedef bgi::rstar<16, 4> parameters;
  typedef my_indexable<std::vector<box> > indexable_getter;

  // boxes
  std::vector<box> boxes;

  // create some boxes
  for (unsigned i = 0; i < 10; ++i) {
    // add a box
    boxes.push_back(box(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f)));
  }

  // display boxes
  std::cout << "generated boxes:" << std::endl;
  BOOST_FOREACH (box const& b, boxes)
    std::cout << bg::wkt<box>(b) << std::endl;

  // create the rtree
  parameters params;
  indexable_getter ind(boxes);
  bgi::rtree<value, parameters, indexable_getter> rtree(params, ind);

  // fill the spatial index
  for (size_t i = 0; i < boxes.size(); ++i) rtree.insert(i);

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
  BOOST_FOREACH (value i, result_s)
    std::cout << bg::wkt<box>(boxes[i]) << std::endl;

  std::cout << "knn query point:" << std::endl;
  std::cout << bg::wkt<point>(point(0, 0)) << std::endl;
  std::cout << "knn query result:" << std::endl;
  BOOST_FOREACH (value i, result_n)
    std::cout << bg::wkt<box>(boxes[i]) << std::endl;

  return 0;
}
