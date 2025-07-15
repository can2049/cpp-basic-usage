#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

// Define namespace aliases to make code more concise
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

// Define 2D point type
// using Point = bg::model::point<double, 2, bg::cs::cartesian>;
using Point = bg::model::d2::point_xy<double>;

using Segment = bg::model::segment<Point>;
// Define polyline type
// using Polyline = std::vector<Point>;
using Polyline = bg::model::linestring<Point>;

// Define R-tree value type (use segment directly)
using ValueType = Segment;

// Calculate shortest distance from point to segment
double distancePointToSegment(const Point& point, const Segment& segment) {
  return bg::distance(point, segment);
}

// Calculate shortest distance from point to polyline (brute force method)
double distancePointToPolylineBruteForce(const Point& point,
                                         const Polyline& polyline) {
  if (polyline.size() < 2) return std::numeric_limits<double>::max();

  double minDistance = std::numeric_limits<double>::max();

  for (size_t i = 0; i < polyline.size() - 1; ++i) {
    Segment segment(polyline[i], polyline[i + 1]);
    double distance = distancePointToSegment(point, segment);
    minDistance = std::min(minDistance, distance);
  }

  return minDistance;
}

// Calculate shortest distance from point to polyline using R-tree
double distancePointToPolylineRTree(
    const Point& point, const bgi::rtree<ValueType, bgi::rstar<16>>& rtree) {
  // Query the nearest segment to the point
  std::vector<ValueType> results;
  rtree.query(bgi::nearest(point, 1), std::back_inserter(results));

  if (results.empty()) return std::numeric_limits<double>::max();

  return distancePointToSegment(point, results[0]);
}

// Generate random polyline
Polyline generateRandomPolyline(int numPoints, double minCoord,
                                double maxCoord) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(minCoord, maxCoord);

  Polyline polyline;
  for (int i = 0; i < numPoints; ++i) {
    polyline.push_back(Point(dis(gen), dis(gen)));
  }

  return polyline;
}

// Generate random query points
std::vector<Point> generateRandomPoints(int numPoints, double minCoord,
                                        double maxCoord) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(minCoord, maxCoord);

  std::vector<Point> points;
  for (int i = 0; i < numPoints; ++i) {
    points.push_back(Point(dis(gen), dis(gen)));
  }

  return points;
}

// Build R-tree
bgi::rtree<ValueType, bgi::rstar<16>> buildRTree(const Polyline& polyline) {
  bgi::rtree<ValueType, bgi::rstar<16>> rtree;

  for (size_t i = 0; i < polyline.size() - 1; ++i) {
    Segment segment(polyline[i], polyline[i + 1]);
    rtree.insert(segment);
  }

  return rtree;
}

// Performance test function
void performanceTest(const Polyline& polyline, int numQueryPoints) {
  // Build R-tree
  auto buildStart = std::chrono::high_resolution_clock::now();
  auto rtree = buildRTree(polyline);
  auto buildEnd = std::chrono::high_resolution_clock::now();

  std::cout << "R-tree build time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(buildEnd -
                                                                     buildStart)
                   .count()
            << " ms\n";

  // Generate random query points
  auto queryPoints = generateRandomPoints(numQueryPoints, -100.0, 100.0);

  // Brute force test
  auto bruteForceStart = std::chrono::high_resolution_clock::now();
  for (const auto& point : queryPoints) {
    distancePointToPolylineBruteForce(point, polyline);
  }
  auto bruteForceEnd = std::chrono::high_resolution_clock::now();

  // R-tree test
  auto rtreeStart = std::chrono::high_resolution_clock::now();
  for (const auto& point : queryPoints) {
    distancePointToPolylineRTree(point, rtree);
  }
  auto rtreeEnd = std::chrono::high_resolution_clock::now();

  // Output performance comparison
  std::cout << "Number of polyline points: " << polyline.size() << "\n";
  std::cout << "Number of query points: " << numQueryPoints << "\n";
  std::cout << "Brute force time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                   bruteForceEnd - bruteForceStart)
                   .count()
            << " ms\n";
  std::cout << "R-tree query time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(rtreeEnd -
                                                                     rtreeStart)
                   .count()
            << " ms\n";

  // Validate result correctness
  int errors = 0;
  for (int i = 0; i < std::min(10, numQueryPoints); ++i) {
    double bf = distancePointToPolylineBruteForce(queryPoints[i], polyline);
    double rt = distancePointToPolylineRTree(queryPoints[i], rtree);
    if (std::abs(bf - rt) > 1e-10) {
      errors++;
      std::cout << "Discrepancy found for point " << i << ": "
                << "BF=" << bf << ", RT=" << rt << "\n";
    }
  }
  std::cout << "Validation errors: " << errors << "\n";
}

int main() {
  std::cout << "=========== Point to Polyline Distance using R-tree Demo "
               "===========\n";

  // Basic example
  std::cout << "\n--- Basic Example ---\n";
  Polyline samplePolyline = {
      {0.0, 0.0}, {1.0, 1.0}, {2.0, 0.0}, {3.0, 1.0}, {4.0, 0.0}};

  // Build R-tree
  auto rtree = buildRTree(samplePolyline);

  // Query example points
  std::vector<Point> sampleQueryPoints = {
      {0.5, 0.5},   // Near the first segment
      {2.0, 1.0},   // In the middle position
      {4.5, 0.5},   // Near the last segment
      {2.0, -1.0},  // Below the polyline
  };

  for (size_t i = 0; i < sampleQueryPoints.size(); ++i) {
    const Point& point = sampleQueryPoints[i];

    double distBruteForce =
        distancePointToPolylineBruteForce(point, samplePolyline);
    double distRTree = distancePointToPolylineRTree(point, rtree);

    std::cout << "Query Point " << i + 1 << " (" << bg::get<0>(point) << ", "
              << bg::get<1>(point) << "):\n";
    std::cout << "  Brute Force Distance: " << distBruteForce << "\n";
    std::cout << "  R-Tree Distance: " << distRTree << "\n\n";
  }

  // Performance tests
  std::cout << "\n--- Performance Test: Small Polyline ---\n";
  Polyline smallPolyline = generateRandomPolyline(100, -100.0, 100.0);
  performanceTest(smallPolyline, 1000);

  std::cout << "\n--- Performance Test: Medium Polyline ---\n";
  Polyline mediumPolyline = generateRandomPolyline(1000, -100.0, 100.0);
  performanceTest(mediumPolyline, 1000);

  std::cout << "\n--- Performance Test: Large Polyline ---\n";
  Polyline largePolyline = generateRandomPolyline(10000, -100.0, 100.0);
  performanceTest(largePolyline, 1000);

  return 0;
}
