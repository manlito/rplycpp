#include <functional>
#include <vector>
#include "rplycpp.hpp"
#include "my_types.hpp"

using namespace myproject;

int main(int argc, char *argv[])
{
  // Some sample data type from your app logic
  Surface<PointXYZRGB> cloud;
  PointXYZRGB point;
  point.x = 5.0; point.y = 0; point.z = 0;
  point.red = 255; point.green = 0; point.blue = 0;
  cloud.points.push_back(point);
  point.x = 0; point.y = 5.0; point.z = 0;
  point.red = 0; point.green = 255; point.blue = 0;
  cloud.points.push_back(point);
  point.x = 0; point.y = 0; point.z = 5.0;
  point.red = 0; point.green = 0; point.blue = 255;
  cloud.points.push_back(point);
  point.x = 5.0; point.y = 5.0; point.z = 5.0;
  point.red = 255; point.green = 255; point.blue = 255;
  cloud.points.push_back(point);

  rplycpp::PLYWriter writer;
  writer.Open(RPLYCPP_SAMPLE_OUTPUT_SIMPLE_PLY);

  // Create header
  std::vector<rplycpp::PLYProperty> properties;
  rplycpp::PLYProperty property;

  property.type = rplycpp::PLYDataType::PLY_FLOAT;
  property.name = "x";
  properties.push_back(property);
  property.name = "y";
  properties.push_back(property);
  property.name = "z";
  properties.push_back(property);

  property.type = rplycpp::PLYDataType::PLY_UCHAR;
  property.name = "red";
  properties.push_back(property);
  property.name = "green";
  properties.push_back(property);
  property.name = "blue";
  properties.push_back(property);
  writer.AddElement("vertex", properties, cloud.points.size());

  // Add the data IN THE SAME ORDER!
  for (const auto &point : cloud.points) {
    writer.AddRow(std::vector<double> {
                    point.x, point.y, point.z,
                    static_cast<double>(point.red),
                    static_cast<double>(point.green),
                    static_cast<double>(point.blue)
                  });
  }

  // Close the file
  writer.Close();
}
