#include <iostream>

struct Point1 {
  int x;
};

struct Point2 {
  int x, y;
};

struct Point3 {
  int x, y, z;
};

struct Point4 {
  int x, y, z, w;
};

struct Point5 {
  int x, y, z, w, v;
};

int main() {
  std::cout << "sizeof(Point1): " << sizeof(Point1) << std::endl;
  std::cout << "alignof(Point1): " << alignof(Point1) << std::endl;
  std::cout << "sizeof(Point2): " << sizeof(Point2) << std::endl;
  std::cout << "alignof(Point2): " << alignof(Point2) << std::endl;
  std::cout << "sizeof(Point3): " << sizeof(Point3) << std::endl;
  std::cout << "alignof(Point3): " << alignof(Point3) << std::endl;
  std::cout << "sizeof(Point4): " << sizeof(Point4) << std::endl;
  std::cout << "alignof(Point4): " << alignof(Point4) << std::endl;
  std::cout << "sizeof(Point5): " << sizeof(Point5) << std::endl;
  std::cout << "alignof(Point5): " << alignof(Point5) << std::endl;
}
