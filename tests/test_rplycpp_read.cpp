#include <functional>
#include <vector>
#include "gtest/gtest.h"
#include "rplycpp.hpp"

using namespace rplycpp;

TEST(PlyReader, ReadPointCloud) {
  PLYReader reader;
  bool read_status = reader.Open(RPLYCPP_TEST_SIMPLE_PLY_XYZRGBNormal);
  EXPECT_EQ(read_status, true) << "Could not open " << RPLYCPP_TEST_SIMPLE_PLY_XYZRGBNormal;

  EXPECT_EQ(reader.GetElements().size(), 1) << "Incorrect number of elements";
}

TEST(PlyReader, ReadSurface) {
  PLYReader reader;
  bool read_status = reader.Open(RPLYCPP_TEST_SIMPLE_PLY_SURFACE);
  EXPECT_EQ(read_status, true) << "Could not open " << RPLYCPP_TEST_SIMPLE_PLY_SURFACE;

  EXPECT_EQ(reader.GetElements().size(), 2) << "Incorrect number of elements";
  EXPECT_EQ(reader.GetElements()[0].GetProperties().size(), 6) << "Incorrect number of properties";
  EXPECT_EQ(reader.GetElements()[1].GetProperties().size(), 1) << "Incorrect number of properties";

  // You can use whatever store your programs uses
  std::vector<std::vector<double>> vertices_matrix, faces_matrix;
  auto vertices_handler = [&vertices_matrix](const std::vector<double> &vertices) {
    vertices_matrix.push_back(vertices);
  };
  auto faces_handler = [&faces_matrix](const std::vector<double> faces) {
    faces_matrix.push_back(faces);
  };
  std::vector<PLYReadHandler> handlers;
  handlers.push_back(vertices_handler);
  handlers.push_back(faces_handler);
  read_status = reader.Read(handlers);
  EXPECT_EQ(read_status, true) << "Could not read " << RPLYCPP_TEST_SIMPLE_PLY_SURFACE;

  EXPECT_EQ(vertices_matrix[1][1], 0.1) << "Incorrect vertex value";
  EXPECT_FLOAT_EQ(vertices_matrix.size(), 4) << "Incorrect number of vertices";
  EXPECT_FLOAT_EQ(faces_matrix.size(), 3) << "Incorrect number of faces";
}
