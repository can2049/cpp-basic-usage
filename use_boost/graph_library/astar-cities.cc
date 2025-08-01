

//
//=======================================================================
// Copyright (c) 2004 Kristopher Beevers
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <boost/algorithm/string/join.hpp>
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
struct Location {
  float y, x;  // lat, long
};

template <class Name, class LocMap>
class CityWriter {
 public:
  CityWriter(Name n, LocMap l, float _minx, float _maxx, float _miny,
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
class TimeWriter {
 public:
  TimeWriter(WeightMap w) : wm_(w) {}
  template <class Edge>
  void operator()(std::ostream& out, const Edge& e) const {
    out << "[label=\"" << wm_[e] << "\", fontsize=\"11\"]";
  }

 private:
  WeightMap wm_;
};

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class DistanceHeuristic : public boost::astar_heuristic<Graph, CostType> {
 public:
  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

  DistanceHeuristic(LocMap l, Vertex goal) : m_location(l), m_goal(goal) {}

  CostType operator()(Vertex u) {
    CostType dx = m_location[m_goal].x - m_location[u].x;
    CostType dy = m_location[m_goal].y - m_location[u].y;
    return ::sqrt(dx * dx + dy * dy);
  }

 private:
  LocMap m_location;
  Vertex m_goal;
};

struct FoundGoal {};  // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class AstarGoalVisitor : public boost::astar_visitor<> {
 public:
  AstarGoalVisitor(Vertex goal) : m_goal(goal) {}

  template <class Graph>
  void examine_vertex(Vertex u, [[maybe_unused]] Graph& g) {
    if (u == m_goal) throw FoundGoal();
  }

 private:
  Vertex m_goal;
};

int main() {
  // specify some types
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                                boost::no_property,
                                boost::property<boost::edge_weight_t, float> >
      Graph;
  typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
  typedef Graph::vertex_descriptor Vertex;

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
  };
  const char* name[] = {"Troy",      "Lake Placid", "Plattsburgh", "Massena",
                        "Watertown", "Utica",       "Syracuse",    "Rochester",
                        "Buffalo",   "Ithaca",      "Binghamton",  "Woodstock",
                        "New York"};
  Location locations[] = {// lat/long
                          {42.73, 73.68}, {44.28, 73.99}, {44.70, 73.46},
                          {44.93, 74.89}, {43.97, 75.91}, {43.10, 75.23},
                          {43.04, 76.14}, {43.17, 77.61}, {42.89, 78.86},
                          {42.44, 76.50}, {42.10, 75.91}, {42.04, 74.11},
                          {40.67, 73.94}};

  static_assert(std::size(name) == std::size(locations),
                "name and locations arrays must be the same size");

  std::pair<int, int> edge_array[] = {
      {Troy, Utica},           {Troy, LakePlacid},
      {Troy, Plattsburgh},     {LakePlacid, Plattsburgh},
      {Plattsburgh, Massena},  {LakePlacid, Massena},
      {Massena, Watertown},    {Watertown, Utica},
      {Watertown, Syracuse},   {Utica, Syracuse},
      {Syracuse, Rochester},   {Rochester, Buffalo},
      {Syracuse, Ithaca},      {Ithaca, Binghamton},
      {Ithaca, Rochester},     {Binghamton, Troy},
      {Binghamton, Woodstock}, {Binghamton, NewYork},
      {Syracuse, Binghamton},  {Woodstock, Troy},
      {Woodstock, NewYork}};

  float weights[] = {// estimated travel time (mins)
                     96, 134, 143, 65,  115, 133, 117, 116, 74, 56, 84,
                     73, 69,  70,  116, 147, 173, 183, 74,  71, 124};

  static_assert(std::size(edge_array) == std::size(weights),
                "edge_array and weights arrays must be the same size");

  // create graph
  Graph graph(std::size(name));
  WeightMap weightmap = get(boost::edge_weight, graph);
  for (std::size_t j = 0; j < std::size(edge_array); ++j) {
    const auto& [city1, city2] = edge_array[j];
    auto [edge_des, inserted] = add_edge(city1, city2, graph);
    weightmap[edge_des] = weights[j];
  }

  // pick random start/goal
  boost::mt19937 gen(std::time(0));
  auto start = random_vertex(graph, gen);
  auto goal = random_vertex(graph, gen);

  std::cout << "Start vertex: " << name[start] << std::endl;
  std::cout << "Goal vertex: " << name[goal] << std::endl;

  std::ofstream dotfile;
  dotfile.open("test-astar-cities.dot");
  write_graphviz(dotfile, graph,
                 CityWriter<const char**, Location*>(
                     name, locations, 73.46, 78.86, 40.67, 44.93, 480, 400),
                 TimeWriter<WeightMap>(weightmap));

  std::vector<Graph::vertex_descriptor> pred(boost::num_vertices(graph));
  std::vector<float> distance(boost::num_vertices(graph));
  try {
    // call astar named parameter interface
    auto heuristic =
        DistanceHeuristic<Graph, float, Location*>(locations, goal);

    auto predecessor_p = make_iterator_property_map(
        pred.begin(), get(boost::vertex_index, graph));

    auto distance_p = boost::make_iterator_property_map(
        distance.begin(), get(boost::vertex_index, graph));

    auto params = predecessor_map(predecessor_p)
                      .distance_map(distance_p)
                      .visitor(AstarGoalVisitor<Vertex>(goal));

    boost::astar_search_tree(graph, start, heuristic, params);
  } catch (FoundGoal fg) {  // found a path to the goal
    std::vector<Vertex> shortest_path;
    for (auto v = goal;; v = pred[v]) {
      shortest_path.push_back(v);
      if (pred[v] == v) {
        break;
      }
    }
    std::cout << "Shortest path from " << name[start] << " to " << name[goal]
              << ": ";
    std::vector<std::string> path_names;
    for (auto it = shortest_path.rbegin(); it != shortest_path.rend(); ++it) {
      path_names.push_back(name[*it]);
    }
    std::cout << boost::join(path_names, " -> ") << "\n"
              << "Total travel time: " << distance[goal] << "\n";
    return 0;
  }

  std::cout << "Didn't find a path from " << name[start] << "to" << name[goal]
            << "!" << std::endl;
  return 0;
}
