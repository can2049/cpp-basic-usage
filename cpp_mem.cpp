#include <iostream>

struct Point {
  int x;
  int y;
};

Point *newPoint(int x, int y) {
  Point p{.x = x, .y = y};
  return &p;  //悬垂指针
}

int main() {
  int values[3] = {1, 2, 3};
  std::cout << values[0] << "," << values[3] << std::endl;  //缓冲区溢出

  Point *p1 = (Point *)malloc(sizeof(Point));
  std::cout << p1->x << "," << p1->y << std::endl;  //使用未初始化内存

  Point *p2 = newPoint(10, 10);  //悬垂指针
  delete p2;                     //非法释放内存

  p1 = NULL;
  std::cout << p1->x << std::endl;  //对空指针解引用
  return 0;
}
