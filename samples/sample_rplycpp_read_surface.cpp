#include <functional>
#include <vector>
#include <iostream>
#include "rplycpp.hpp"
#include "my_types.hpp"

using namespace myproject;

int main(int argc, char *argv[])
{
  // Some randome data type from your app logic
  Surface<PointXYZRGB> cloud;

  rplycpp::PLYReader reader;
  reader.Open(RPLYCPP_SAMPLE_SURFACE_PLY);

  auto vertices_handler = [&cloud](const std::vector<double> &vertex) {
    // This is your app logic, just remember there is an array of double with all row values
    // Input ply has 9 vertex properties: x y z nx ny nz red green blue
    PointXYZRGB point;
    point.x = vertex[0];
    point.y = vertex[1];
    point.z = vertex[2];
    point.red = vertex[3];
    point.green = vertex[4];
    point.blue = vertex[5];
    cloud.points.push_back(point);
  };
  auto faces_handler = [&cloud](const std::vector<double> &vertex_indexes) {
    // Input ply has vertex indices is an array (first element is number of elements)
    Face face;
    face.resize(static_cast<size_t>(vertex_indexes[0]));
    face[0] = vertex_indexes[1];
    face[1] = vertex_indexes[2];
    face[2] = vertex_indexes[3];
    cloud.faces.push_back(face);
  };

  // Register the handlers
  std::vector<rplycpp::PLYReadHandler> handlers;
  handlers.push_back(vertices_handler);
  handlers.push_back(faces_handler);

  // Read. This will be invoke your handlers when needed
  reader.Read(handlers);

  // Optional close (destructor will try it anyway)
  reader.Close();

  // Print
  for (const auto &point : cloud.points) {
    std::cout << point;
  }
  for (const auto &face : cloud.faces) {
    std::cout << face;
  }

  return 0;
}
