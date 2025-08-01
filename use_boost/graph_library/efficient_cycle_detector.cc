#include <glog/logging.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <iostream>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;

// Efficient cycle detection: manual DFS control
class manual_cycle_detector {
 private:
  enum Color { WHITE, GRAY, BLACK };
  std::vector<Color> colors;
  int edge_count = 0;

 public:
  bool has_cycle_dfs(const Graph& g) {
    colors.assign(boost::num_vertices(g), WHITE);
    edge_count = 0;

    for (auto v = 0u; v < boost::num_vertices(g); ++v) {
      if (colors[v] == WHITE) {
        if (dfs_visit(g, v)) {
          return true;  // Found cycle, return immediately
        }
      }
    }
    return false;
  }

  int get_edge_count() const { return edge_count; }

 private:
  bool dfs_visit(const Graph& g, int u) {
    colors[u] = GRAY;  // Mark as currently visiting

    auto [ei, ei_end] = boost::out_edges(u, g);
    for (; ei != ei_end; ++ei) {
      ++edge_count;
      int v = boost::target(*ei, g);

      LOG(INFO) << "Examining edge " << edge_count << ": " << u << "->" << v;

      if (colors[v] == GRAY) {
        LOG(INFO) << "Found back edge (cycle): " << u << "->" << v;
        return true;  // Found cycle!
      }

      if (colors[v] == WHITE && dfs_visit(g, v)) {
        return true;  // Found cycle in subtree
      }
    }

    colors[u] = BLACK;  // Mark as completed
    return false;
  }
};

class FoundGoal : public std::exception {
 public:
  FoundGoal(const std::string& msg) : msg_(msg) {}
  const char* what() const noexcept override { return msg_.c_str(); }

 private:
  std::string msg_;
};

// State-based visitor (no exceptions) - with proper termination
struct state_based_cycle_detector : public boost::dfs_visitor<> {
  mutable bool should_terminate = false;
  mutable int edge_count = 0;

  // 1. 在发现顶点时检查终止条件
  template <class Vertex, class Graph>
  void discover_vertex(Vertex v, const Graph& g) const {
    LOG(INFO) << "Discover vertex: " << v;
    if (should_terminate) {
      LOG(INFO) << "Early termination - skip vertex " << v;
      return;
    }
  }

  // 2. 在检查边时检查终止条件
  template <class Edge, class Graph>
  void examine_edge(Edge e, const Graph& g) const {
    if (should_terminate) {
      LOG(INFO) << "Skip edge due to termination flag";
      return;  // 跳过边的处理
    }

    ++edge_count;
    LOG(INFO) << "Examining edge " << edge_count << ": " << boost::source(e, g)
              << "->" << boost::target(e, g);
  }

  // 3. 在处理树边时检查终止条件
  template <class Edge, class Graph>
  void tree_edge(Edge e, const Graph& g) const {
    if (should_terminate) {
      LOG(INFO) << "Skip tree edge due to termination";
      return;
    }
    LOG(INFO) << "Processing tree edge: " << boost::source(e, g) << "->"
              << boost::target(e, g) << std::endl;
  }

  // 4. 发现回边时设置终止标志
  template <class Edge, class Graph>
  void back_edge(Edge e, const Graph& g) const {
    LOG(INFO) << "Found cycle! Back edge: " << boost::source(e, g) << "->"
              << boost::target(e, g);
    throw FoundGoal("Found cycle! back edge");
  }

  // 5. 在完成顶点时检查终止条件
  template <class Vertex, class Graph>
  void finish_vertex(Vertex v, const Graph& g) const {
    if (should_terminate) {
      LOG(INFO) << "Skip finish vertex " << v << " due to termination";
      return;
    }
    LOG(INFO) << "Finish vertex: " << v;
  }
};

int main() {
  Graph g(4);
  boost::add_edge(0, 1, g);
  boost::add_edge(1, 2, g);
  boost::add_edge(2, 0, g);  // First cycle
  boost::add_edge(1, 3, g);
  boost::add_edge(3, 1, g);  // Second cycle

  LOG(INFO) << "Graph has 5 edges, contains 2 cycles";

  // Method 1: Manual DFS (most efficient)
  LOG(INFO) << "=== Method 1: Manual DFS (most efficient) ===";
  manual_cycle_detector detector1;
  bool has_cycle = detector1.has_cycle_dfs(g);
  LOG(INFO) << "Result: " << (has_cycle ? "Has cycle" : "No cycle");
  LOG(INFO) << "Examined " << detector1.get_edge_count() << " edges";

  // Method 2: State-based visitor
  LOG(INFO) << "=== Method 2: State-controlled visitor ===";
  state_based_cycle_detector detector2;
  try {
    boost::depth_first_search(g, boost::visitor(detector2));
  } catch (FoundGoal& e) {
    LOG(INFO) << " Found goal! what: " << e.what();
  }
  LOG(INFO) << "Examined " << detector2.edge_count << " edges";
  LOG(INFO) << "Termination flag: " << std::boolalpha
            << detector2.should_terminate;

  return 0;
}
