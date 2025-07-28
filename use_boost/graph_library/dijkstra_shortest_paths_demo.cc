#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <iostream>
#include <map>
#include <vector>

int main() {
  //------------------------------------------------
  // 1. 定义图类型
  //------------------------------------------------
  using GraphTraits =
      boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
  using Graph = boost::adjacency_list<
      boost::vecS, boost::vecS, boost::directedS,
      boost::no_property,                            // 顶点属性
      boost::property<boost::edge_weight_t, double>  // 边属性
      >;
  using vertex_t = GraphTraits::vertex_descriptor;
  using edge_t = GraphTraits::edge_descriptor;

  //------------------------------------------------
  // 2. 构造一个小图： 0 -> 1 -> 2 -> 3
  //------------------------------------------------
  Graph my_graph;
  const vertex_t v0 = boost::add_vertex(my_graph);
  const vertex_t v1 = boost::add_vertex(my_graph);
  const vertex_t v2 = boost::add_vertex(my_graph);
  const vertex_t v3 = boost::add_vertex(my_graph);

  // 真实边权存在 std::map<edge_t,double> 中，而不是图对象里
  std::map<edge_t, double> edge_weight_map;

  auto add_edge_with_weight = [&](vertex_t u, vertex_t v, double w) {
    auto [edge, ok] = boost::add_edge(u, v, my_graph);
    edge_weight_map[edge] = w;
  };
  add_edge_with_weight(v0, v1, 2.0);
  add_edge_with_weight(v1, v2, 3.0);
  add_edge_with_weight(v2, v3, 1.0);

  //------------------------------------------------
  // 3. 把 std::map 伪装成 ReadablePropertyMap
  //------------------------------------------------
  auto weight_pm = boost::associative_property_map<std::map<edge_t, double>>(
      edge_weight_map);

  //------------------------------------------------
  // 4. 准备算法输出缓存
  //------------------------------------------------
  boost::vector_property_map<double> dist(num_vertices(my_graph));
  boost::vector_property_map<vertex_t> pred(num_vertices(my_graph));

  //------------------------------------------------
  // 5. 运行 Dijkstra
  //------------------------------------------------
  dijkstra_shortest_paths(
      my_graph, v0,
      boost::weight_map(weight_pm).distance_map(dist).predecessor_map(pred));

  //------------------------------------------------
  // 6. 打印结果
  //------------------------------------------------
  auto [vi, ve] = boost::vertices(my_graph);
  for (auto it = vi; it != ve; ++it) {
    const vertex_t& v = *it;

    assert(dist[v] == get(dist, v));
    assert(pred[v] == get(pred, v));

    std::cout << "v: " << v << " dist: " << get(dist, v)
              << ", pred: " << get(pred, v) << '\n';
  }

  return 0;
}
