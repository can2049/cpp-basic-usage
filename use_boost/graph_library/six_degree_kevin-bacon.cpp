//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

template <typename Type>
class bacon_number_recorder : public boost::bfs_visitor<> {
 public:
  // C++17: Explicit deduction guide makes CTAD work better
  bacon_number_recorder(std::vector<Type>& dist) : dist_map_(dist) {}

  template <typename Edge, typename Graph>
  void tree_edge(Edge e, const Graph& g) const {
    auto source_vertex = boost::source(e, g);
    auto target_vertex = boost::target(e, g);
    dist_map_[target_vertex] = dist_map_[source_vertex] + 1;
  }

 private:
  std::vector<Type>& dist_map_;
};

int main(int argc, const char** argv) {
  std::ifstream datafile(argc >= 2 ? argv[1] : "./kevin-bacon.dat");
  if (!datafile) {
    std::cerr << "No ./kevin-bacon.dat file" << std::endl;
    return EXIT_FAILURE;
  }

  using Graph =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                            boost::property<boost::vertex_name_t, std::string>,
                            boost::property<boost::edge_name_t, std::string>>;
  Graph my_graph;

  auto actor_name = boost::get(boost::vertex_name, my_graph);
  auto connecting_movie = boost::get(boost::edge_name, my_graph);

  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  using NameVertexMap = std::map<std::string, Vertex>;
  NameVertexMap actors;

  for (std::string line; std::getline(datafile, line);) {
    boost::char_delimiters_separator<char> sep(false, "", ";");
    boost::tokenizer<> line_toks(line, sep);
    auto i = line_toks.begin();
    std::string actors_name = *i++;
    Vertex u, v;
    auto [pos, inserted] = actors.insert(std::make_pair(actors_name, Vertex()));
    if (inserted) {
      u = boost::add_vertex(my_graph);
      actor_name[u] = actors_name;
      pos->second = u;
    } else {
      u = pos->second;
    }

    std::string movie_name = *i++;

    auto [pos2, inserted2] = actors.insert(std::make_pair(*i, Vertex()));
    if (inserted2) {
      v = boost::add_vertex(my_graph);
      actor_name[v] = *i;
      pos2->second = v;
    } else {
      v = pos2->second;
    }

    auto [e, edge_inserted] = boost::add_edge(u, v, my_graph);
    if (edge_inserted) {
      connecting_movie[e] = movie_name;
    }
  }

  std::vector<int> bacon_number(boost::num_vertices(my_graph), -1);

  Vertex start_vertex = actors["Kevin Bacon"];
  bacon_number[start_vertex] = 0;

  // C++17 CTAD: Compiler automatically deduces template parameters
  boost::breadth_first_search(
      my_graph, start_vertex,
      boost::visitor(bacon_number_recorder(bacon_number)));

  // C++17: Structured bindings and range-based for loop
  for (auto [vi, ve] = boost::vertices(my_graph); vi != ve; ++vi) {
    std::cout << actor_name[*vi] << " has a Bacon number of "
              << bacon_number[*vi] << std::endl;
  }

  return 0;
}
