#include <boost/geometry.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>
#include <fstream>
#include <iostream>
#include <vector>

namespace bg = boost::geometry;

int main() {
  // 定义点类型和几何图形
  typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
  typedef bg::model::polygon<point_t> polygon_t;
  typedef bg::model::linestring<point_t> linestring_t;
  typedef bg::model::ring<point_t> ring_t;

  // 创建几何对象
  polygon_t poly;
  linestring_t line;
  ring_t ring;

  // 定义多边形（五边形）
  bg::append(poly, point_t(100, 100));
  bg::append(poly, point_t(150, 50));
  bg::append(poly, point_t(200, 100));
  bg::append(poly, point_t(180, 180));
  bg::append(poly, point_t(120, 180));
  bg::append(poly, point_t(100, 100));  // 闭合多边形

  // 定义线串
  bg::append(line, point_t(250, 50));
  bg::append(line, point_t(300, 150));
  bg::append(line, point_t(350, 50));
  bg::append(line, point_t(400, 150));

  // 定义环（类似多边形但不自相交）
  bg::append(ring, point_t(450, 100));
  bg::append(ring, point_t(500, 50));
  bg::append(ring, point_t(550, 100));
  bg::append(ring, point_t(450, 100));  // 闭合环

  // 创建SVG文件

  const char* svg_filename = "shapes_with_legend.svg";

  std::ofstream svg(svg_filename);
  bg::svg_mapper<point_t> mapper(svg, 600, 300);

  // 绘制几何图形（带样式）
  mapper.add(poly);
  mapper.map(poly, "fill-opacity:0.5;fill:blue;stroke:black;stroke-width:2", 5);

  mapper.add(line);
  mapper.map(line, "stroke:red;stroke-width:3", 5);

  mapper.add(ring);
  mapper.map(ring,
             "fill-opacity:0.3;fill:green;stroke:purple;stroke-width:2;stroke-"
             "dasharray:5,5",
             5);

  // 添加图例
  mapper.text(point_t(50, 30),
              "Legend:", "font-family:Arial;font-size:12px;font-weight:bold");
  mapper.text(point_t(50, 50), "Blue: Polygon (filled)",
              "font-family:Arial;font-size:10px;fill:blue");
  mapper.text(point_t(50, 70), "Red: Linestring",
              "font-family:Arial;font-size:10px;fill:red");
  mapper.text(point_t(50, 90), "Green: Ring (dashed stroke)",
              "font-family:Arial;font-size:10px;fill:green");

  // 添加标题
  mapper.text(
      point_t(250, 20), "Boost.Geometry Basic Shapes Example",
      "font-family:Arial;font-size:14px;font-weight:bold;text-anchor:middle");

  std::cout << "SVG file created: " << svg_filename << std::endl;

  return 0;
}
