#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <iostream>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;

// 示例1: 只在发现顶点时触发的访问器
struct vertex_discoverer : public boost::base_visitor<vertex_discoverer> {
  typedef boost::on_discover_vertex event_filter;  // 关键：指定事件类型

  template <class Vertex, class GraphType>
  void operator()(Vertex v, GraphType&) {
    std::cout << "发现顶点: " << v << std::endl;
  }
};

// 示例2: 只在完成顶点时触发的访问器
struct vertex_finisher : public boost::base_visitor<vertex_finisher> {
  typedef boost::on_finish_vertex event_filter;  // 不同的事件类型

  template <class Vertex, class GraphType>
  void operator()(Vertex v, GraphType&) {
    std::cout << "完成顶点: " << v << std::endl;
  }
};

// 示例3: 只在树边时触发的访问器
struct tree_edge_printer : public boost::base_visitor<tree_edge_printer> {
  typedef boost::on_tree_edge event_filter;  // 边事件类型

  template <class Edge, class GraphType>
  void operator()(Edge e, GraphType& g) {
    std::cout << "树边: " << boost::source(e, g) << " -> "
              << boost::target(e, g) << std::endl;
  }
};

// 示例4: 错误的事件类型演示
struct wrong_event_visitor : public boost::base_visitor<wrong_event_visitor> {
  typedef boost::on_tree_edge event_filter;  // 指定为边事件

  template <class Vertex, class GraphType>
  void operator()(Vertex v, GraphType&) {
    // 这个函数不会被调用，因为 event_filter 不匹配
    std::cout << "这不会被打印: " << v << std::endl;
  }
};

int main() {
  Graph g(4);
  boost::add_edge(0, 1, g);
  boost::add_edge(0, 2, g);
  boost::add_edge(1, 3, g);
  boost::add_edge(2, 3, g);

  std::cout << "=== 演示 event_filter 的作用 ===" << std::endl;
  std::cout << std::endl;

  // 组合多个访问器
  auto combined_visitor = boost::make_dfs_visitor(
      std::make_pair(vertex_discoverer(),
                     std::make_pair(vertex_finisher(), tree_edge_printer())));

  std::cout << "DFS 遍历（从顶点 0 开始）：" << std::endl;
  boost::depth_first_search(g, boost::visitor(combined_visitor));

  return 0;
}
