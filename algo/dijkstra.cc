#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

typedef pair<int, string> P;  // (当前路径长度, 当前节点)

tuple<int, vector<string>> dijkstra(
    unordered_map<string, vector<pair<string, int>>> &graph,
    const string &start, const string &target) {
  priority_queue<P, vector<P>, greater<P>> pq;  // 最小堆
  unordered_map<string, int> dist;              // 记录最短路径
  unordered_map<string, string> prev;           // 记录路径

  for (const auto &[node, edge] : graph) {
    dist[node] = numeric_limits<int>::max();
  }
  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    auto [cur_dist, node] = pq.top();
    pq.pop();

    if (node == target) {
      break;
    }

    for (const auto &[neighbor, weight] : graph[node]) {
      int new_dist = cur_dist + weight;
      if (new_dist < dist[neighbor]) {
        dist[neighbor] = new_dist;
        prev[neighbor] = node;
        pq.push({new_dist, neighbor});
      }
    }
  }

  // 生成最短路径
  vector<string> path;
  for (string node = target; prev.find(node) != prev.end(); node = prev[node]) {
    path.push_back(node);
  }
  path.push_back(start);
  reverse(path.begin(), path.end());

  return {dist[target], path};
}

int main() {
  unordered_map<string, vector<pair<string, int>>> graph = {
      {"A", {{"B", 1}, {"C", 4}}},
      {"B", {{"A", 1}, {"C", 2}, {"D", 5}}},
      {"C", {{"A", 4}, {"B", 2}, {"D", 1}}},
      {"D", {{"B", 5}, {"C", 1}}}};

  auto [distance, path] = dijkstra(graph, "A", "D");

  cout << "Shortest distance: " << distance << "\nPath: ";
  for (const auto &node : path) {
    cout << node << " ";
  }
  cout << endl;

  return 0;
}
