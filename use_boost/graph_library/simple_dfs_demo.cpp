#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

int main() {
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    Graph g(3);
    boost::add_edge(0, 1, g);
    boost::add_edge(1, 2, g);
    
    std::cout << "=== 基本 DFS (无访问器) ===" << std::endl;
    boost::depth_first_search(g);
    std::cout << "DFS completed" << std::endl;
    
    return 0;
}
