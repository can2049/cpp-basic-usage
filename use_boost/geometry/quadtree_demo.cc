#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

namespace bg = boost::geometry;

// ---------------- 基本类型 ----------------
using point_t = bg::model::d2::point_xy<double>;
using box_t = bg::model::box<point_t>;
using value_t = std::pair<point_t, std::size_t>;  // <坐标, 用户ID>

// ---------------- Quadtree ----------------
class QuadTree {
  static constexpr std::size_t max_elements_ = 4;
  static constexpr std::size_t max_depth_ = 8;

  box_t boundary_;
  std::vector<value_t> points_;
  std::unique_ptr<QuadTree> children_[4];  // 顺序：NW, NE, SW, SE
  std::size_t depth_;

  // 根据索引返回子象限的边界
  box_t make_child_box(std::size_t index) const {
    double min_x = bg::get<0>(boundary_.min_corner());
    double min_y = bg::get<1>(boundary_.min_corner());
    double max_x = bg::get<0>(boundary_.max_corner());
    double max_y = bg::get<1>(boundary_.max_corner());
    double mid_x = (min_x + max_x) / 2.0;
    double mid_y = (min_y + max_y) / 2.0;

    switch (index) {
      case 0:
        return box_t(point_t(min_x, mid_y), point_t(mid_x, max_y));  // NW
      case 1:
        return box_t(point_t(mid_x, mid_y), point_t(max_x, max_y));  // NE
      case 2:
        return box_t(point_t(min_x, min_y), point_t(mid_x, mid_y));  // SW
      case 3:
        return box_t(point_t(mid_x, min_y), point_t(max_x, mid_y));  // SE
    }
    return {};  // never reach
  }

  void subdivide() {
    for (std::size_t i = 0; i < 4; ++i)
      children_[i] = std::make_unique<QuadTree>(make_child_box(i), depth_ + 1);

    // 把当前节点保存的点全部下放到子节点
    for (auto&& v : points_) insert_into_children(v);
    points_.clear();
  }

  void insert_into_children(const value_t& v) {
    for (auto& child : children_)
      if (child && child->contains(v.first)) {
        child->insert(v);
        break;
      }
  }

  bool contains(const point_t& p) const { return bg::within(p, boundary_); }

 public:
  explicit QuadTree(const box_t& b, std::size_t d = 0)
      : boundary_(b), depth_(d) {}

  void insert(const value_t& v) {
    if (!contains(v.first)) return;

    if (children_[0])  // 已分裂：向下传
    {
      insert_into_children(v);
    } else  // 未分裂：先存本地
    {
      points_.push_back(v);
      if (points_.size() > max_elements_ && depth_ < max_depth_) subdivide();
    }
  }

  // 范围查询：把落在查询矩形内的 (点,ID) 收集到 result
  void query(const box_t& range, std::vector<value_t>& result) const {
    if (!bg::intersects(range, boundary_)) return;

    for (const auto& v : points_)
      if (bg::within(v.first, range)) result.push_back(v);

    for (const auto& child : children_)
      if (child) child->query(range, result);
  }

  // 调试用：打印树结构
  void dump(std::ostream& os = std::cout, std::size_t indent = 0) const {
    std::string in(indent, ' ');
    os << in << "Box: (" << bg::wkt(boundary_)
       << ")  Points: " << points_.size() << '\n';
    for (const auto& child : children_)
      if (child) child->dump(os, indent + 2);
  }
};

int main() {
  // 1. 构建一个 100×100 的世界
  box_t world(point_t(0, 0), point_t(100, 100));
  QuadTree qt(world);

  // 2. 随机插入 100 个点
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(0, 100);

  for (std::size_t i = 0; i < 100; ++i) {
    point_t p(dist(rng), dist(rng));
    qt.insert({p, i});
  }

  // 3. 查询中心 20×20 矩形内的所有点
  box_t query_range(point_t(40, 40), point_t(60, 60));
  std::vector<value_t> result;
  qt.query(query_range, result);

  std::cout << "Query hits: " << result.size() << "\n";
  for (auto&& [pt, id] : result)
    std::cout << "  id=" << id << " " << bg::wkt(pt) << "\n";

  // 4. 可选：打印整棵树结构
  // qt.dump();
}
