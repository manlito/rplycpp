#include <functional>
#include <vector>
#include "rplycpp.hpp"
#include "my_types.hpp"

using namespace myproject;

int main(int argc, char *argv[])
{
  // Some sample data type from your app logic
  Surface<PointXYZ> cloud;
  PointXYZ point;
  point.x = 5.0; point.y = 0; point.z = 0;
  cloud.points.push_back(point);
  point.x = 0; point.y = 5.0; point.z = 0;
  cloud.points.push_back(point);
  point.x = 0; point.y = 0; point.z = 5.0;
  cloud.points.push_back(point);
  point.x = 5.0; point.y = 5.0; point.z = 5.0;
  cloud.points.push_back(point);
  Face face(3);
  face[0] = 2; face[1] = 1; face[2] = 0;
  cloud.faces.push_back(face);
  face[0] = 3; face[1] = 1; face[2] = 0;
  cloud.faces.push_back(face);
  face[0] = 3; face[1] = 2; face[2] = 1;
  cloud.faces.push_back(face);

  rplycpp::PLYWriter writer;
  writer.Open(RPLYCPP_SAMPLE_OUTPUT_SURFACE_PLY, rplycpp::PLYStorageMode::PLY_DEFAULT);

  // Create header

  // Properties for first segment: vertex
  std::vector<rplycpp::PLYProperty> properties;
  rplycpp::PLYProperty property;
  property.name = "x";
  property.type = rplycpp::PLYDataType::PLY_FLOAT;
  properties.push_back(property);
  property.name = "y";
  properties.push_back(property);
  property.name = "z";
  properties.push_back(property);
  writer.AddElement("vertex", properties, cloud.points.size());

  // Properties for second segment: faces
  properties.clear();
  property.name = "vertex_indices";
  property.type = rplycpp::PLYDataType::PLY_LIST;
  property.list_length = 3;
  property.list_length_type = rplycpp::PLYDataType::PLY_UCHAR;
  property.list_value_type = rplycpp::PLYDataType::PLY_INT;
  properties.push_back(property);
  writer.AddElement("face", properties, cloud.faces.size());

  // Add the data IN THE SAME ORDER!
  for (const auto &point : cloud.points) {
    writer.AddRow(std::vector<double> {point.x, point.y, point.z});
  }
  for (const auto &face : cloud.faces) {
    writer.AddRow(std::vector<double> {
                    3,
                    static_cast<double>(face[0]),
                    static_cast<double>(face[1]),
                    static_cast<double>(face[2])
                  });
  }

  // Close the file
  writer.Close();
}
