#include <algorithm>
#include <map>
#include <functional>
#include <vector>
#include <iostream>
#include "rplycpp.hpp"
#include "my_types.hpp"

using namespace myproject;

int main(int argc, char *argv[])
{
  // Some random data type from your app logic
  Surface<PointXYZRGB> cloud;

  rplycpp::PLYReader reader;
  reader.Open(RPLYCPP_SAMPLE_SURFACE_PLY);

  // Store for handlers
  std::vector<rplycpp::PLYReadHandler> handlers;

  // Save the index of the property name
  std::map<std::string, size_t> property_to_index;
  auto vertices_handler = [&cloud, &property_to_index](const std::vector<double> &vertex) {
    // In this case, we don't assume any particular ordering
    PointXYZRGB point;
    point.x = vertex[property_to_index["x"]];
    point.y = vertex[property_to_index["y"]];
    point.z = vertex[property_to_index["z"]];
    point.red = static_cast<unsigned char>(vertex[property_to_index["red"]]);
    point.green = static_cast<unsigned char>(vertex[property_to_index["green"]]);
    point.blue = static_cast<unsigned char>(vertex[property_to_index["blue"]]);
    cloud.points.push_back(point);
  };
  auto faces_handler = [&cloud](const std::vector<double> &vertex_indexes) {
    // Check for a valid face
    if (vertex_indexes.size() != 4) {
      return;
    }
    // Input ply has vertex indices is an array (first element is number of elements)
    Face face;
    face.resize(static_cast<size_t>(vertex_indexes[0]));
    face[0] = vertex_indexes[1];
    face[1] = vertex_indexes[2];
    face[2] = vertex_indexes[3];
    cloud.faces.push_back(face);
  };

  // Suppose we don't know the input PLY, but that we want:
  // elements vertex and vertex_indices
  // Also, vertex are supposed to have x, y, z (order does not matter)
  // and for face, we want a list type of type vertex_indices
  for (const rplycpp::PLYElement element: reader.GetElements()) {
    std::string element_name = element.GetName();
    std::cout << "Property: " << element_name << std::endl;
    if (element_name == "vertex") {
      // Some validation
      std::vector<std::string> required_properties {"x", "y", "z"};
      std::vector<std::string> property_names;
      for (const auto &property : element.GetProperties()) {
        property_names.push_back(property.name);
      }
      for (const auto &required_property : required_properties) {
        if (std::find(property_names.begin(), property_names.end(), required_property) == property_names.end()) {
          std::cerr << "Missing property " << required_property << " in " << element_name << std::endl;
          reader.Close();
          return EXIT_FAILURE;
        }
      }
      // Populate map from property name to index
      for (size_t i = 0; i < element.GetProperties().size(); ++i) {
        const auto &property = element.GetProperties()[i];
        property_to_index[property.name] = i;
      }
      handlers.push_back(vertices_handler);
    } else if (element_name == "face") {
      const auto &properties = element.GetProperties();
      if (properties.size() != 1 || properties[0].name != "vertex_indices") {
        std::cerr << "Missing property vertex_indices in face" << std::endl;
        reader.Close();
        return EXIT_FAILURE;
      }
      if (properties[0].type != rplycpp::PLYDataType::PLY_LIST) {
        std::cerr << "Vertex indices should be a list" << std::endl;
        reader.Close();
        return EXIT_FAILURE;
      }
      handlers.push_back(faces_handler);
    }
  }

  // Read. This will be invoke your handlers when needed
  reader.Read(handlers);

  // Optional close (destructor will try it anyway)
  reader.Close();

  // Print
  for (const auto &point : cloud.points) {
    std::cout << point;
  }
  for (const auto &point : cloud.points) {
    std::cout << point;
  }

  return 0;
}
