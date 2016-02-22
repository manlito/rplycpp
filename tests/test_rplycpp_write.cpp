#include <functional>
#include <vector>
#include "gtest/gtest.h"
#include "rplycpp.hpp"

using namespace rplycpp;

TEST(PlyWriter, WritePointCloud) {
  PLYWriter writer;
  bool write_status = writer.Open(RPLYCPP_TEST_OUTPUT_SIMPLE_PLY);
  EXPECT_EQ(write_status, true) << "Could not open " << RPLYCPP_TEST_OUTPUT_SIMPLE_PLY;

  std::vector<PLYProperty> properties;
  PLYProperty property;
  property.name = "x";
  properties.push_back(property);
  property.name = "y";
  properties.push_back(property);
  property.name = "z";
  properties.push_back(property);
  property.name = "red";
  property.type = PLYDataType::PLY_UCHAR;
  properties.push_back(property);
  property.name = "green";
  properties.push_back(property);
  property.name = "blue";
  properties.push_back(property);
  write_status = writer.AddElement("vertex", properties, 4);
  EXPECT_EQ(write_status, true) << "Could add element";

  std::vector<double> row_1 { 0,  0,  0, 128, 128,   0};
  std::vector<double> row_2 {10,  0,  0,   0,  77, 220};
  std::vector<double> row_3 { 0, 10,  0,   0, 255,   0};
  std::vector<double> row_4 {10, 10,  0, 255,   0, 128};
  write_status = writer.AddRow(row_1);
  write_status = writer.AddRow(row_2);
  write_status = writer.AddRow(row_3);
  write_status = writer.AddRow(row_4);

  write_status = writer.Close();
  EXPECT_EQ(write_status, true) << "Could not close file";
}

TEST(PlyWriter, WriteSurface) {
  PLYWriter writer;
  bool write_status = writer.Open(RPLYCPP_TEST_OUTPUT_SURFACE_PLY);
  EXPECT_EQ(write_status, true) << "Could not open " << RPLYCPP_TEST_OUTPUT_SURFACE_PLY;

  std::vector<PLYProperty> properties;
  PLYProperty property;
  property.name = "x";
  properties.push_back(property);
  property.name = "y";
  properties.push_back(property);
  property.name = "z";
  properties.push_back(property);
  property.name = "red";
  property.type = PLYDataType::PLY_UCHAR;
  properties.push_back(property);
  property.name = "green";
  properties.push_back(property);
  property.name = "blue";
  properties.push_back(property);
  write_status = writer.AddElement("vertex", properties, 4);
  EXPECT_EQ(write_status, true) << "Could not write vertex header";

  properties.clear();
  property.name = "vertex_indices";
  property.type = PLYDataType::PLY_LIST;
  property.list_length = 3;
  property.list_length_type = PLYDataType::PLY_UCHAR;
  property.list_value_type = PLYDataType::PLY_INT;
  properties.push_back(property);
  write_status = writer.AddElement("face", properties, 3);
  EXPECT_EQ(write_status, true) << "Could not add face header";

  std::vector<double> row_1 {0, 0, 1, 255, 128, 0};
  std::vector<double> row_2 {0, 1, 0, 0, 128, 255};
  std::vector<double> row_3 {1, 0, 0, 0, 255, 128};
  std::vector<double> row_4 {1, 1, 1, 128, 0, 255};
  write_status = writer.AddRow(row_1);
  write_status = writer.AddRow(row_2);
  write_status = writer.AddRow(row_3);
  write_status = writer.AddRow(row_4);

  std::vector<double> row_5 {3, 0, 1, 2};
  std::vector<double> row_6 {3, 1, 2, 3};
  std::vector<double> row_7 {3, 0, 1, 3};
  write_status = writer.AddRow(row_5);
  write_status = writer.AddRow(row_6);
  write_status = writer.AddRow(row_7);

  write_status = writer.Close();
  EXPECT_EQ(write_status, true) << "Could not close file";

}


