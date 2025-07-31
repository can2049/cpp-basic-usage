#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <iostream>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// 自定义访问器 - 打印发现顺序
struct print_visitor : public boost::default_dfs_visitor {
  void discover_vertex(Vertex v, const Graph&) const {
    std::cout << "Discovered vertex: " << v << std::endl;
  }

  void finish_vertex(Vertex v, const Graph&) const {
    std::cout << "Finished vertex: " << v << std::endl;
  }
};

// 另一个访问器 - 只关心back_edge
struct simple_cycle_detector : public boost::default_dfs_visitor {
  simple_cycle_detector(bool& cycle) : has_cycle(cycle) {}

  template <class Edge>
  void back_edge(Edge, const Graph&) const {
    has_cycle = true;
    std::cout << "Found back edge - cycle detected!" << std::endl;
  }

  bool& has_cycle;
};

int main() {
  // 创建一个简单的图: 0->1->2, 2->0 (形成循环)
  Graph g(3);
  boost::add_edge(0, 1, g);
  boost::add_edge(1, 2, g);
  boost::add_edge(2, 0, g);  // 这条边创建循环

  std::cout << "=== 1. 基本 DFS (无访问器) ===" << std::endl;
  boost::depth_first_search(g);
  std::cout << "DFS completed (no output expected)\n" << std::endl;

  std::cout << "=== 2. 使用打印访问器 ===" << std::endl;
  boost::depth_first_search(g, boost::visitor(print_visitor()));
  std::cout << std::endl;

  std::cout << "=== 3. 使用循环检测访问器 ===" << std::endl;
  bool has_cycle = false;
  boost::depth_first_search(g,
                            boost::visitor(simple_cycle_detector(has_cycle)));
  std::cout << "Has cycle: " << std::boolalpha << has_cycle << std::endl;
  std::cout << std::endl;

  std::cout << "=== 4. 无循环的图 ===" << std::endl;
  Graph g2(3);
  boost::add_edge(0, 1, g2);
  boost::add_edge(1, 2, g2);
  // 不添加 2->0，所以无循环

  has_cycle = false;
  boost::depth_first_search(g2,
                            boost::visitor(simple_cycle_detector(has_cycle)));
  std::cout << "Has cycle: " << std::boolalpha << has_cycle << std::endl;

  return 0;
}
