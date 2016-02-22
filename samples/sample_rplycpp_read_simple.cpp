#include <functional>
#include <vector>
#include <iostream>
#include "rplycpp.hpp"
#include "my_types.hpp"

using namespace myproject;

int main(int argc, char *argv[])
{
  // Some randome data type from your app logic
  Cloud<PointXYZ> cloud;

  rplycpp::PLYReader reader;
  reader.Open(RPLYCPP_SAMPLE_SIMPLE_PLY);

  auto vertices_handler = [&cloud](const std::vector<double> &vertex) {
    // This is your app logic, just remember there is an array of double with all row values
    // Input ply has 9 vertex properties: x y z nx ny nz red green blue
    // But we're only using the first three... is up to you!
    PointXYZ point;
    point.x = vertex[0];
    point.y = vertex[1];
    point.z = vertex[2];
    cloud.points.push_back(point);
  };

  // Register the handlers
  std::vector<rplycpp::PLYReadHandler> handlers {vertices_handler};

  // Read. This will be invoke your handlers when needed
  reader.Read(handlers);

  // Optional close (destructor will try it anyway)
  reader.Close();

  // Print
  for (const auto &point : cloud.points) {
    std::cout << point;
  }

  return 0;
}
