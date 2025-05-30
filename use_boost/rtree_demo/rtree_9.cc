// https://live.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_examples/index_stored_in_mapped_file_using_boost_interprocess.html

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <iostream>

namespace bi = boost::interprocess;
namespace bg = boost::geometry;
namespace bgm = bg::model;
namespace bgi = bg::index;

int main() {
  typedef bgm::point<float, 2, bg::cs::cartesian> point_t;

  typedef point_t value_t;
  typedef bgi::linear<32, 8> params_t;
  typedef bgi::indexable<value_t> indexable_t;
  typedef bgi::equal_to<value_t> equal_to_t;
  typedef bi::allocator<value_t, bi::managed_mapped_file::segment_manager>
      allocator_t;
  typedef bgi::rtree<value_t, params_t, indexable_t, equal_to_t, allocator_t>
      rtree_t;

  {
    bi::managed_mapped_file file(bi::open_or_create, "data.bin", 1024 * 1024);
    allocator_t alloc(file.get_segment_manager());
    rtree_t* rtree_ptr = file.find_or_construct<rtree_t>("rtree")(
        params_t(), indexable_t(), equal_to_t(), alloc);

    std::cout << rtree_ptr->size() << std::endl;

    rtree_ptr->insert(point_t(1.0, 1.0));
    rtree_ptr->insert(point_t(2.0, 2.0));

    std::cout << rtree_ptr->size() << std::endl;
  }

  {
    bi::managed_mapped_file file(bi::open_or_create, "data.bin", 1024 * 1024);
    allocator_t alloc(file.get_segment_manager());
    rtree_t* rtree_ptr = file.find_or_construct<rtree_t>("rtree")(
        params_t(), indexable_t(), equal_to_t(), alloc);

    std::cout << rtree_ptr->size() << std::endl;

    rtree_ptr->insert(point_t(3.0, 3.0));
    rtree_ptr->insert(point_t(4.0, 4.0));

    std::cout << rtree_ptr->size() << std::endl;
  }

  return 0;
}
