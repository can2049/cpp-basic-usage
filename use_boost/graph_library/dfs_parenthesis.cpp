//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
//  Sample output
//  DFS parenthesis:
//  (0(2(3(4(11)4)3)2)0)

#include <boost/config.hpp>
#include <cassert>
#include <iostream>
#include <utility>

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/depth_first_search.hpp"
#include "boost/graph/visitors.hpp"

struct open_paren : public boost::base_visitor<open_paren> {
  typedef boost::on_discover_vertex event_filter;
  template <class Vertex, class Graph>
  void operator()(Vertex v, Graph&) {
    std::cout << "(" << v;
  }
};
struct close_paren : public boost::base_visitor<close_paren> {
  typedef boost::on_finish_vertex event_filter;
  template <class Vertex, class Graph>
  void operator()(Vertex v, Graph&) {
    std::cout << v << ")";
  }
};

int main(int, char*[]) {
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
      Graph;
  typedef std::pair<int, int> E;
  E edge_array[] = {{0, 2}, {1, 1}, {1, 3}, {2, 1}, {2, 3},
                    {3, 1}, {3, 4}, {4, 0}, {4, 1}};

  Graph g(std::begin(edge_array), std::end(edge_array), 5);

  std::cout << "DFS parenthesis:" << std::endl;
  depth_first_search(
      g,
      visitor(make_dfs_visitor(std::make_pair(open_paren(), close_paren()))));
  std::cout << std::endl;
  return 0;
}
