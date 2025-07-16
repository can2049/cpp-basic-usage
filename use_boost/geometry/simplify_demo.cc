#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::linestring<point_t> linestring_t;

void plot_linestrings(const linestring_t& original,
                      const linestring_t& simplified) {
  // Write data to temporary files
  std::ofstream original_file("original_line.dat");
  std::ofstream simplified_file("simplified_line.dat");

  // Write original linestring data
  for (const auto& point : original) {
    original_file << bg::get<0>(point) << " " << bg::get<1>(point) << std::endl;
  }
  original_file.close();

  // Write simplified linestring data
  for (const auto& point : simplified) {
    simplified_file << bg::get<0>(point) << " " << bg::get<1>(point)
                    << std::endl;
  }
  simplified_file.close();

  // Create gnuplot script
  std::ofstream gnuplot_script("plot_script.gp");
  gnuplot_script << "set terminal png size 800,600\n";
  gnuplot_script << "set output 'linestring_comparison.png'\n";
  gnuplot_script << "set title 'Original vs Simplified Linestring'\n";
  gnuplot_script << "set xlabel 'X'\n";
  gnuplot_script << "set ylabel 'Y'\n";
  gnuplot_script << "set grid\n";
  // gnuplot_script << "set key outside\n";  // Put legend outside plot area
  gnuplot_script << "plot 'original_line.dat' with linespoints linewidth 2 "
                    "pointtype 7 linecolor rgb 'blue' title 'Original', ";
  gnuplot_script << "'simplified_line.dat' with linespoints linewidth 2 "
                    "pointtype 5 linecolor rgb 'red' title 'Simplified'\n";
  // gnuplot_script
  //     << "pause -1 'Press any key to continue...'\n";  // Keep window open
  gnuplot_script.close();

  // Execute gnuplot
  system("gnuplot plot_script.gp");

  // Clean up temporary files
  system("rm original_line.dat simplified_line.dat plot_script.gp");

  std::cout << "Plot saved as 'linestring_comparison.png'" << std::endl;
}

int main() {
  // 创建原始线串
  linestring_t line;
  line.push_back(point_t(1.1, 1.1));
  line.push_back(point_t(2.5, 2.1));
  line.push_back(point_t(3.1, 3.1));
  line.push_back(point_t(4.9, 1.1));
  line.push_back(point_t(5.5, 1.1));
  line.push_back(point_t(6.0, 0.5));

  // 简化线串
  linestring_t simplified;
  double tolerance = 0.5;  // 简化容差
  bg::simplify(line, simplified, tolerance);

  // Plot both linestrings
  plot_linestrings(line, simplified);

  return 0;
}
