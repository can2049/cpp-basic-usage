

//
//=======================================================================
// Copyright (c) 2004 Kristopher Beevers
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/random.hpp>
#include <boost/random.hpp>
#include <cmath>  // for sqrt
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

// auxiliary types
struct location {
  float y, x;  // lat, long
};
typedef float cost;

template <class Name, class LocMap>
class city_writer {
 public:
  city_writer(Name n, LocMap l, float _minx, float _maxx, float _miny,
              float _maxy, unsigned int _ptx, unsigned int _pty)
      : name(n),
        loc(l),
        minx(_minx),
        maxx(_maxx),
        miny(_miny),
        maxy(_maxy),
        ptx(_ptx),
        pty(_pty) {}
  template <class Vertex>
  void operator()(std::ostream& out, const Vertex& v) const {
    float px = 1 - (loc[v].x - minx) / (maxx - minx);
    float py = (loc[v].y - miny) / (maxy - miny);
    out << "[label=\"" << name[v] << "\", pos=\""
        << static_cast<unsigned int>(ptx * px) << ","
        << static_cast<unsigned int>(pty * py) << "\", fontsize=\"11\"]";
  }

 private:
  Name name;
  LocMap loc;
  float minx, maxx, miny, maxy;
  unsigned int ptx, pty;
};

template <class WeightMap>
class time_writer {
 public:
  time_writer(WeightMap w) : wm(w) {}
  template <class Edge>
  void operator()(std::ostream& out, const Edge& e) const {
    out << "[label=\"" << wm[e] << "\", fontsize=\"11\"]";
  }

 private:
  WeightMap wm;
};

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class distance_heuristic : public boost::astar_heuristic<Graph, CostType> {
 public:
  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
  distance_heuristic(LocMap l, Vertex goal) : m_location(l), m_goal(goal) {}
  CostType operator()(Vertex u) {
    CostType dx = m_location[m_goal].x - m_location[u].x;
    CostType dy = m_location[m_goal].y - m_location[u].y;
    return ::sqrt(dx * dx + dy * dy);
  }

 private:
  LocMap m_location;
  Vertex m_goal;
};

struct found_goal {};  // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor {
 public:
  astar_goal_visitor(Vertex goal) : m_goal(goal) {}
  template <class Graph>
  void examine_vertex(Vertex u, [[maybe_unused]] Graph& g) {
    if (u == m_goal) throw found_goal();
  }

 private:
  Vertex m_goal;
};

int main() {
  // specify some types
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                                boost::no_property,
                                boost::property<boost::edge_weight_t, cost> >
      mygraph_t;
  typedef boost::property_map<mygraph_t, boost::edge_weight_t>::type WeightMap;
  typedef mygraph_t::vertex_descriptor vertex;
  typedef std::pair<int, int> edge;

  // specify data
  enum nodes {
    Troy,
    LakePlacid,
    Plattsburgh,
    Massena,
    Watertown,
    Utica,
    Syracuse,
    Rochester,
    Buffalo,
    Ithaca,
    Binghamton,
    Woodstock,
    NewYork,
    N
  };
  const char* name[] = {"Troy",      "Lake Placid", "Plattsburgh", "Massena",
                        "Watertown", "Utica",       "Syracuse",    "Rochester",
                        "Buffalo",   "Ithaca",      "Binghamton",  "Woodstock",
                        "New York"};
  location locations[] = {// lat/long
                          {42.73, 73.68}, {44.28, 73.99}, {44.70, 73.46},
                          {44.93, 74.89}, {43.97, 75.91}, {43.10, 75.23},
                          {43.04, 76.14}, {43.17, 77.61}, {42.89, 78.86},
                          {42.44, 76.50}, {42.10, 75.91}, {42.04, 74.11},
                          {40.67, 73.94}};

  static_assert(std::size(name) == std::size(locations),
                "name and locations arrays must be the same size");

  edge edge_array[] = {
      edge(Troy, Utica),           edge(Troy, LakePlacid),
      edge(Troy, Plattsburgh),     edge(LakePlacid, Plattsburgh),
      edge(Plattsburgh, Massena),  edge(LakePlacid, Massena),
      edge(Massena, Watertown),    edge(Watertown, Utica),
      edge(Watertown, Syracuse),   edge(Utica, Syracuse),
      edge(Syracuse, Rochester),   edge(Rochester, Buffalo),
      edge(Syracuse, Ithaca),      edge(Ithaca, Binghamton),
      edge(Ithaca, Rochester),     edge(Binghamton, Troy),
      edge(Binghamton, Woodstock), edge(Binghamton, NewYork),
      edge(Syracuse, Binghamton),  edge(Woodstock, Troy),
      edge(Woodstock, NewYork)};

  cost weights[] = {// estimated travel time (mins)
                    96, 134, 143, 65,  115, 133, 117, 116, 74, 56, 84,
                    73, 69,  70,  116, 147, 173, 183, 74,  71, 124};

  static_assert(std::size(edge_array) == std::size(weights),
                "edge_array and weights arrays must be the same size");

  // create graph
  mygraph_t my_graph(N);
  WeightMap weightmap = get(boost::edge_weight, my_graph);
  for (std::size_t j = 0; j < std::size(edge_array); ++j) {
    auto [edge_des, inserted] =
        add_edge(edge_array[j].first, edge_array[j].second, my_graph);
    weightmap[edge_des] = weights[j];
  }

  // pick random start/goal
  boost::mt19937 gen(std::time(0));
  vertex start = random_vertex(my_graph, gen);
  vertex goal = random_vertex(my_graph, gen);

  std::cout << "Start vertex: " << name[start] << std::endl;
  std::cout << "Goal vertex: " << name[goal] << std::endl;

  std::ofstream dotfile;
  dotfile.open("test-astar-cities.dot");
  write_graphviz(dotfile, my_graph,
                 city_writer<const char**, location*>(
                     name, locations, 73.46, 78.86, 40.67, 44.93, 480, 400),
                 time_writer<WeightMap>(weightmap));

  std::vector<mygraph_t::vertex_descriptor> parent(
      boost::num_vertices(my_graph));
  std::vector<cost> distance(boost::num_vertices(my_graph));
  try {
    // call astar named parameter interface
    auto heuristic =
        distance_heuristic<mygraph_t, cost, location*>(locations, goal);
    auto params =
        predecessor_map(boost::make_iterator_property_map(
                            parent.begin(), get(boost::vertex_index, my_graph)))
            .distance_map(make_iterator_property_map(
                distance.begin(), get(boost::vertex_index, my_graph)))
            .visitor(astar_goal_visitor<vertex>(goal));

    boost::astar_search_tree(my_graph, start, heuristic, params);
  } catch (found_goal fg) {  // found a path to the goal
    std::list<vertex> shortest_path;
    for (vertex v = goal;; v = parent[v]) {
      shortest_path.push_front(v);
      if (parent[v] == v) break;
    }
    std::cout << "Shortest path from " << name[start] << " to " << name[goal]
              << ": ";
    std::list<vertex>::iterator spi = shortest_path.begin();
    std::cout << name[start];
    for (++spi; spi != shortest_path.end(); ++spi)
      std::cout << " -> " << name[*spi];
    std::cout << std::endl
              << "Total travel time: " << distance[goal] << std::endl;
    return 0;
  }

  std::cout << "Didn't find a path from " << name[start] << "to" << name[goal]
            << "!" << std::endl;
  return 0;
}
