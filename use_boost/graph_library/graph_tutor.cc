//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <algorithm>  // for std::for_each
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/utility.hpp>  // for boost::tie
#include <iostream>           // for std::cout
#include <utility>            // for std::pair

// create a typedef for the Graph type
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, float> >
    Graph;

template <class Graph>
struct exercise_vertex {
  exercise_vertex(Graph& g_, const char name_[]) : g(g_), name(name_) {}

  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

  void operator()(const Vertex& v) const {
    auto vertex_id = boost::get(boost::vertex_index, g);
    std::cout << "vertex: " << name[boost::get(vertex_id, v)] << std::endl;

    // Print degree information
    std::cout << "\tdegree: " << boost::degree(v, g) << std::endl;

    // Write out the outgoing edges
    std::cout << "\tout-degree: " << boost::out_degree(v, g) << std::endl;
    std::cout << "\tout-edges: ";
    for (auto [oi, oe] = boost::out_edges(v, g); oi != oe; ++oi) {
      auto e = *oi;
      auto src = boost::source(e, g);
      auto targ = boost::target(e, g);
      std::cout << "(" << name[boost::get(vertex_id, src)] << ","
                << name[boost::get(vertex_id, targ)] << ") ";
    }
    std::cout << std::endl;
    std::cout << "\tout-edge count: "
              << std::distance(boost::out_edges(v, g).first,
                               boost::out_edges(v, g).second)
              << std::endl;

    // Write out the incoming edges
    std::cout << "\tin-degree: " << boost::in_degree(v, g) << std::endl;
    std::cout << "\tin-edges: ";
    for (auto [ii, ie] = boost::in_edges(v, g); ii != ie; ++ii) {
      auto e = *ii;
      auto src = boost::source(e, g);
      auto targ = boost::target(e, g);
      std::cout << "(" << name[boost::get(vertex_id, src)] << ","
                << name[boost::get(vertex_id, targ)] << ") ";
    }
    std::cout << std::endl;
    std::cout << "\tin-edge count: "
              << std::distance(boost::in_edges(v, g).first,
                               boost::in_edges(v, g).second)
              << std::endl;

    // Write out all adjacent vertices
    std::cout << "\tadjacent vertices: ";
    for (auto [ai, ae] = boost::adjacent_vertices(v, g); ai != ae; ++ai)
      std::cout << name[boost::get(vertex_id, *ai)] << " ";
    std::cout << std::endl;

    std::cout << "\tadjacent vertex count: "
              << std::distance(boost::adjacent_vertices(v, g).first,
                               boost::adjacent_vertices(v, g).second)
              << std::endl;
  }
  Graph& g;
  const char* name;
};

int main(int, char*[]) {
  // Make convenient labels for the vertices
  enum { A, B, C, D, E, N };
  const int num_vertices = N;
  const char name[] = "ABCDE";

  // writing out the edges in the graph
  std::pair<int, int> edge_array[] = {{A, B}, {A, D}, {C, A}, {D, C},
                                      {C, E}, {B, D}, {D, E}, {B, E}};

  // average transmission delay (in milliseconds) for each connection
  float transmission_delay[] = {1.2, 4.5, 2.6, 0.4, 5.2, 1.8, 3.3, 9.1};

  // declare a graph object, adding the edges and edge properties
  std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << "\n";
  static_assert(std::size(edge_array) == std::size(transmission_delay));
  Graph g(std::begin(edge_array), std::end(edge_array),
          std::begin(transmission_delay), num_vertices);

  boost::property_map<Graph, boost::vertex_index_t>::type vertex_id =
      boost::get(boost::vertex_index, g);
  boost::property_map<Graph, boost::edge_weight_t>::type trans_delay =
      boost::get(boost::edge_weight, g);

  std::cout << "==> vertices(g) = ";
  for (auto [vi, ve] = boost::vertices(g); vi != ve; ++vi) {
    std::cout << name[boost::get(vertex_id, *vi)] << " ";
  }
  std::cout << std::endl;
  std::cout << "==> total vertices in graph: " << boost::num_vertices(g)
            << std::endl;

  std::cout << "==> edges(g) = ";
  boost::graph_traits<Graph>::edge_iterator ei, ei_end;
  for (auto [ei, ei_end] = boost::edges(g); ei != ei_end; ++ei) {
    std::cout << "(" << name[boost::get(vertex_id, source(*ei, g))] << ","
              << name[boost::get(vertex_id, target(*ei, g))] << ") ";
  }
  std::cout << std::endl;

  {
    auto [vi, ve] = boost::vertices(g);
    std::for_each(vi, ve, exercise_vertex<Graph>(g, name));
  }

  std::cout << "==> Graphviz output:\n";
  std::map<std::string, std::string> graph_attr, vertex_attr, edge_attr;
  graph_attr["size"] = "3,3";
  graph_attr["rankdir"] = "LR";
  graph_attr["ratio"] = "fill";
  vertex_attr["shape"] = "circle";

  boost::write_graphviz(
      std::cout, g, boost::make_label_writer(name),
      boost::make_label_writer(trans_delay),
      boost::make_graph_attributes_writer(graph_attr, vertex_attr, edge_attr));

  return 0;
}
