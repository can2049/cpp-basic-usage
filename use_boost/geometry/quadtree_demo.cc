#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

namespace bg = boost::geometry;

// ---------------- 基本类型 ----------------
using point = bg::model::d2::point_xy<double>;
using box = bg::model::box<point>;
using value = std::pair<point, std::size_t>;  // <坐标, 用户ID>

// ---------------- Quadtree ----------------
class QuadTree {
  static constexpr std::size_t max_elements = 4;
  static constexpr std::size_t max_depth = 8;

  box boundary;
  std::vector<value> points;
  std::unique_ptr<QuadTree> children[4];  // 顺序：NW, NE, SW, SE
  std::size_t depth;

  // 根据索引返回子象限的边界
  box make_child_box(std::size_t index) const {
    double min_x = bg::get<0>(boundary.min_corner());
    double min_y = bg::get<1>(boundary.min_corner());
    double max_x = bg::get<0>(boundary.max_corner());
    double max_y = bg::get<1>(boundary.max_corner());
    double mid_x = (min_x + max_x) / 2.0;
    double mid_y = (min_y + max_y) / 2.0;

    switch (index) {
      case 0:
        return box(point(min_x, mid_y), point(mid_x, max_y));  // NW
      case 1:
        return box(point(mid_x, mid_y), point(max_x, max_y));  // NE
      case 2:
        return box(point(min_x, min_y), point(mid_x, mid_y));  // SW
      case 3:
        return box(point(mid_x, min_y), point(max_x, mid_y));  // SE
    }
    return {};  // never reach
  }

  void subdivide() {
    for (std::size_t i = 0; i < 4; ++i)
      children[i] = std::make_unique<QuadTree>(make_child_box(i), depth + 1);

    // 把当前节点保存的点全部下放到子节点
    for (auto&& v : points) insert_into_children(v);
    points.clear();
  }

  void insert_into_children(const value& v) {
    for (auto& child : children)
      if (child && child->contains(v.first)) {
        child->insert(v);
        break;
      }
  }

  bool contains(const point& p) const { return bg::within(p, boundary); }

 public:
  explicit QuadTree(const box& b, std::size_t d = 0) : boundary(b), depth(d) {}

  void insert(const value& v) {
    if (!contains(v.first)) return;

    if (children[0])  // 已分裂：向下传
    {
      insert_into_children(v);
    } else  // 未分裂：先存本地
    {
      points.push_back(v);
      if (points.size() > max_elements && depth < max_depth) subdivide();
    }
  }

  // 范围查询：把落在查询矩形内的 (点,ID) 收集到 result
  void query(const box& range, std::vector<value>& result) const {
    if (!bg::intersects(range, boundary)) return;

    for (const auto& v : points)
      if (bg::within(v.first, range)) result.push_back(v);

    for (const auto& child : children)
      if (child) child->query(range, result);
  }

  // 调试用：打印树结构
  void dump(std::ostream& os = std::cout, std::size_t indent = 0) const {
    std::string in(indent, ' ');
    os << in << "Box: (" << bg::wkt(boundary) << ")  Points: " << points.size()
       << '\n';
    for (const auto& child : children)
      if (child) child->dump(os, indent + 2);
  }
};

int main() {
  // 1. 构建一个 100×100 的世界
  box world(point(0, 0), point(100, 100));
  QuadTree qt(world);

  // 2. 随机插入 100 个点
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(0, 100);

  for (std::size_t i = 0; i < 100; ++i) {
    point p(dist(rng), dist(rng));
    qt.insert({p, i});
  }

  // 3. 查询中心 20×20 矩形内的所有点
  box query_range(point(40, 40), point(60, 60));
  std::vector<value> result;
  qt.query(query_range, result);

  std::cout << "Query hits: " << result.size() << "\n";
  for (auto&& [pt, id] : result)
    std::cout << "  id=" << id << " " << bg::wkt(pt) << "\n";

  // 4. 可选：打印整棵树结构
  // qt.dump();
}
