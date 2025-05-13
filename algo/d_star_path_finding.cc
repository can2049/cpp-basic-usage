#include <algorithm>
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// 定义地图大小
const int ROWS = 5;
const int COLS = 5;

// 节点结构体
struct Node {
  int x, y;
  int cost = 1;  // 默认可通过
  int g = 0;     // 到目标的实际代价
  int rhs = 0;   // 到目标的启发式代价

  bool operator<(const Node& other) const {
    return (g + heuristic(x, y)) >
           (other.g + other.heuristic(other.x, other.y));
  }

  int heuristic(int x1, int y1, int x2 = 0, int y2 = 0) const {
    return abs(x1 - x2) + abs(y1 - y2);  // 曼哈顿距离
  }
};

// 地图类
class Grid {
 public:
  vector<vector<Node>> grid;

  Grid() {
    grid.resize(ROWS, vector<Node>(COLS));
    for (int i = 0; i < ROWS; ++i)
      for (int j = 0; j < COLS; ++j) grid[i][j] = {i, j};
  }

  void setObstacle(int x, int y) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) grid[x][y].cost = 0;
  }

  void printGrid(Node start, Node goal, vector<Node> path) {
    cout << "Grid State:\n";
    for (int i = 0; i < ROWS; ++i) {
      for (int j = 0; j < COLS; ++j) {
        bool inPath = false;
        for (auto p : path)
          if (p.x == i && p.y == j) {
            inPath = true;
            break;
          }
        if (i == start.x && j == start.y)
          cout << " S ";
        else if (i == goal.x && j == goal.y)
          cout << " G ";
        else if (grid[i][j].cost == 0)
          cout << " # ";
        else if (inPath)
          cout << " * ";
        else
          cout << " . ";
      }
      cout << "\n";
    }
    cout << endl;
  }
};

// 优先队列比较器
struct Compare {
  bool operator()(Node a, Node b) {
    return (a.g + a.heuristic(a.x, a.y)) > (b.g + b.heuristic(b.x, b.y));
  }
};

class DStarLite {
 private:
  Grid& grid;
  Node start, goal;
  priority_queue<Node, vector<Node>, Compare> openList;
  unordered_map<int, bool> visited;

 public:
  DStarLite(Grid& g, Node s, Node t) : grid(g), start(s), goal(t) { init(); }

  void init() {
    // 初始化所有节点的 g 和 rhs
    for (int i = 0; i < ROWS; ++i)
      for (int j = 0; j < COLS; ++j) {
        grid.grid[i][j].g = INT_MAX;
        grid.grid[i][j].rhs = INT_MAX;
      }
    grid.grid[goal.x][goal.y].rhs = 0;
    insert(goal, 0);
  }

  void insert(Node n, int k) { openList.push(n); }

  bool isConsistent(Node n) {
    return grid.grid[n.x][n.y].g == grid.grid[n.x][n.y].rhs;
  }

  void computeShortestPath() {
    while (!openList.empty()) {
      Node u = openList.top();
      openList.pop();

      if (u.g < u.rhs) {
        u.g = INT_MAX;
        cout << "Update g(" << u.x << "," << u.y << ") = " << u.g << endl;
      } else if (u.g > u.rhs) {
        u.g = u.rhs;
        cout << "Update g(" << u.x << "," << u.y << ") = " << u.g << endl;
        for (auto neighbor : getNeighbors(u)) {
          updateVertex(neighbor);
        }
      }

      if (isConsistent(u)) {
        cout << "Node (" << u.x << "," << u.y << ") is consistent\n";
      } else {
        cout << "Node (" << u.x << "," << u.y << ") is inconsistent\n";
      }
    }
  }

  vector<Node> getNeighbors(Node n) {
    vector<Node> neighbors;
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; ++i) {
      int nx = n.x + dx[i];
      int ny = n.y + dy[i];
      if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS &&
          grid.grid[nx][ny].cost != 0) {
        neighbors.push_back(grid.grid[nx][ny]);
      }
    }
    return neighbors;
  }

  void updateVertex(Node n) {
    if (n.x != goal.x || n.y != goal.y) {
      n.rhs = INT_MAX;
      for (auto neighbor : getNeighbors(n)) {
        int newRhs = neighbor.g + neighbor.cost;
        if (newRhs < n.rhs) {
          n.rhs = newRhs;
        }
      }
    }
    if (isConsistent(n)) {
      cout << "Node (" << n.x << "," << n.y << ") is consistent, skipping\n";
    } else {
      insert(n, n.rhs);
      cout << "Inserting (" << n.x << "," << n.y
           << ") into openList with rhs=" << n.rhs << endl;
    }
  }

  vector<Node> getPath() {
    vector<Node> path;
    Node current = start;
    path.push_back(current);
    while (!(current.x == goal.x && current.y == goal.y)) {
      vector<Node> neighbors = getNeighbors(current);
      Node next = *min_element(
          neighbors.begin(), neighbors.end(), [this](Node a, Node b) {
            return grid.grid[a.x][a.y].g < grid.grid[b.x][b.y].g;
          });
      current = next;
      path.push_back(current);
    }
    return path;
  }

  void addObstacle(int x, int y) {
    grid.setObstacle(x, y);
    Node n = grid.grid[x][y];
    n.rhs = INT_MAX;
    updateVertex(n);
    cout << "Added obstacle at (" << x << "," << y << ")\n";
  }
};

int main() {
  Grid grid;
  Node start = {0, 0};
  Node goal = {4, 4};

  DStarLite dsl(grid, start, goal);

  cout << "Initial Path Planning:\n";
  dsl.computeShortestPath();
  vector<Node> path = dsl.getPath();
  grid.printGrid(start, goal, path);

  cout << "Adding obstacle at (2,2)\n";
  dsl.addObstacle(2, 2);
  dsl.computeShortestPath();
  path = dsl.getPath();
  grid.printGrid(start, goal, path);

  return 0;
}
