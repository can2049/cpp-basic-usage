#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_set>
#include <vector>

// 定义坐标点结构
struct Point {
  int x, y;

  Point(int x = 0, int y = 0) : x(x), y(y) {}

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point& other) const { return !(*this == other); }
};

// Overload operator<< for Point to enable printing
std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}

// 为Point定义哈希函数，用于unordered_set
namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& p) const {
    // 使用黄金比例数（或任意大质数）来避免哈希冲突
    constexpr size_t prime = 2654435769;  // 2^32 * (sqrt(5)-1)/2
    size_t h1 = std::hash<int>{}(p.x);
    size_t h2 = std::hash<int>{}(p.y);
    // 组合哈希（确保 (a,b) ≠ (b,a)）
    return h1 ^ (h2 + prime + (h1 << 6) + (h1 >> 2));
  }
};
}  // namespace std

// 定义节点结构，用于A*算法
struct Node {
  Point pt;
  int g;  // 从起点到当前节点的实际距离
  // int h;  // 启发式估计到终点的距离
  int f;  // f = g + h

  Node(Point p, int g, int h) : pt(p), g(g), f(g + h) {}
};

// 比较函数，用于优先队列
struct CompareNodeGt {
  // for std::priority_queue
  bool operator()(const Node& a, const Node& b) const { return a.f > b.f; }
};

// 地图类
class SparseMap {
 private:
  int width, height;
  std::unordered_set<Point> obstacles;
  std::unordered_set<Point> path;
  std::unordered_set<Point> closedSet;
  std::unordered_set<Point> openSet;
  Point start, goal;

 public:
  SparseMap(int w, int h) : width(w), height(h) {}

  void addObstacle(int x, int y) { obstacles.insert(Point(x, y)); }
  void removeObstacle(int x, int y) { obstacles.erase(Point(x, y)); }

  void setStart(int x, int y) { start = Point(x, y); }

  void setGoal(int x, int y) { goal = Point(x, y); }

  void setPath(const std::vector<Point>& p) {
    path.clear();
    for (const auto& point : p) {
      path.insert(point);
    }
  }

  void setClosedSet(const std::unordered_set<Point>& cs) { closedSet = cs; }

  void setOpenSet(const std::unordered_set<Point>& os) { openSet = os; }

  bool isObstacle(int x, int y) const {
    return obstacles.find(Point(x, y)) != obstacles.end();
  }

  bool isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
  }

  void display() const {
// 清屏 (Windows用"cls", Linux/Mac用"clear")
#ifdef _WIN32
    // system("cls");
#else
    // system("clear");
#endif

    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        Point p(x, y);
        if (p == start) {
          std::cout << "S ";
        } else if (p == goal) {
          std::cout << "G ";
        } else if (path.find(p) != path.end()) {
          std::cout << "* ";
        } else if (openSet.find(p) != openSet.end()) {
          std::cout << "o ";
        } else if (closedSet.find(p) != closedSet.end()) {
          std::cout << "x ";
        } else if (isObstacle(x, y)) {
          std::cout << "# ";
        } else {
          std::cout << ". ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Legend: S=Start, G=Goal, *=Path, o=Open, x=Closed, #=Obstacle"
              << std::endl;
  }

  Point getStart() const { return start; }
  Point getGoal() const { return goal; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
};

// 启发式函数 (曼哈顿距离)
// 改为与 moveCost 同尺度的启发函数（曼哈顿距离 * 10）
int heuristic(const Point& a, const Point& b) {
  auto h = (std::abs(a.x - b.x) + std::abs(a.y - b.y)) * 10;
  // auto h = std::hypot(a.x - b.x, a.y - b.y) * 10;
  return h;
}

// A* 算法实现（保留可视化）
std::vector<Point> aStarSearch(SparseMap& map, bool visualize = true) {
  Point start = map.getStart();
  Point goal = map.getGoal();

  // smallest f value first
  std::priority_queue<Node, std::vector<Node>, CompareNodeGt> open_set;

  // global minimal cost from start to current node
  std::unordered_map<Point, int> gScore;
  gScore[start] = 0;

  // 父节点映射，用于重建路径
  std::unordered_map<Point, Point> cameFrom;

  open_set.push({start, 0, heuristic(start, goal)});
  std::unordered_set<Point> closedSet;
  std::unordered_set<Point> open_set_viz{start};

  // 8 个方向
  constexpr std::array<int, 8> dx = {-1, 1, 0, 0, -1, -1, 1, 1};
  constexpr std::array<int, 8> dy = {0, 0, -1, 1, -1, 1, -1, 1};
  constexpr std::array<int, 8> cost = {10, 10, 10, 10, 14, 14, 14, 14};

  while (!open_set.empty()) {
    auto current = open_set.top();  // pop the point with smallest f value
    open_set.pop();
    Point cp = current.pt;  // current point

    // 如果这是一个“过时”节点，跳过
    if (current.g > gScore[cp]) {
      std::cout << "Skipping outdated node: " << cp << std::endl;
      // exit(-1);
      continue;
    }

    // 到达目标
    if (cp == goal) {
      // 重建路径
      std::vector<Point> path;
      Point p = goal;
      while (p != start) {
        path.push_back(p);
        p = cameFrom[p];
      }
      path.push_back(start);
      std::reverse(path.begin(), path.end());
      return path;
    }

    open_set_viz.erase(cp);
    closedSet.insert(cp);  // add current point to closed set

    if (visualize) {
      map.setClosedSet(closedSet);
      map.setOpenSet(open_set_viz);
      map.display();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 扩展邻居
    for (int i = 0; i < 8; ++i) {
      int nx = cp.x + dx[i];
      int ny = cp.y + dy[i];
      if (!map.isValid(nx, ny) || map.isObstacle(nx, ny)) continue;
      Point np(nx, ny);  // next point

      if (closedSet.count(np)) continue;

      int tentativeG = current.g + cost[i];
      // 如果这是更优路径，或者之前未访问
      if (!gScore.count(np) || tentativeG < gScore[np]) {
        gScore[np] = tentativeG;
        cameFrom[np] = cp;
        int h = heuristic(np, goal);
        open_set.push({np, tentativeG, h});  // add the point to open set
        open_set_viz.insert(np);
      }
    }
  }

  // 未找到路径
  return {};
}

int main() {
  // 创建一个20x20的地图
  SparseMap map(20, 20);

  // 设置起点和终点
  map.setStart(1, 1);
  map.setGoal(17, 19);

  // 添加一些障碍物
  for (int i = 5; i < 15; ++i) {
    map.addObstacle(i, 10);
  }
  for (int i = 5; i < 15; ++i) {
    map.addObstacle(10, i);
  }
  // for (int i = 2; i < 18; ++i) {
  //   map.addObstacle(i, 5);
  // }
  for (int i = 3; i < 10; ++i) {
    map.addObstacle(15, i);
  }
  for (int i = 6; i < 10; i++) {
    map.addObstacle(i, 5);
    map.addObstacle(5, i);
  }
  for (int i = 3; i < 17; ++i) {
    map.addObstacle(i, 15);
  }
  map.removeObstacle(9, 10);

  // 运行A*算法并显示可视化
  std::vector<Point> path = aStarSearch(map, true);

  // 显示最终结果
  map.setPath(path);
  map.display();

  if (path.empty()) {
    std::cout << "No path found!" << std::endl;
  } else {
    std::cout << "Path found with length: " << path.size() << std::endl;
  }

  return 0;
}
