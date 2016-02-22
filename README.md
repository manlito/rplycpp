# rplycpp
A C++ Wrapper for RPLy.

The original RPLy library can be found at http://w3.impa.br/~diego/software/rply/.

## What is it

This is a C++11 library that wraps RPLy C library. The general workflow to create and read a PLY is preserved, in order to be efficient. In this way, the library won't create intermediary objects in order to be usefull to read and write huge files. The main features of this library are:

- Supports for both list and scalar properties
- It allows any number of elements
- Use of C++11 lambdas allows easily reading elements

## How to use it

### In your project

This wrapper won't change too much. So, I recommend to embed it as a 3rd party. Feel free to modify the main CMakeLists.txt to fit your distributable code.

### In your code

Reading a PLY files can be done as follows:

```c++
// Surface would be a class you made
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

```

Writing is a little bit more complex. The following writes a surface:

```c++
// Some sample data type from your app logic
Surface<PointXYZ> cloud;
PointXYZ point;
point.x = 5.0; point.y = 0; point.z = 0;
cloud.points.push_back(point);
// ... Add as many points needed
Face face(3);
face[0] = 2; face[1] = 1; face[2] = 0;
cloud.faces.push_back(face);
// ... Add as many faces  needed

rplycpp::PLYWriter writer;
// Note that you can save as ASCII (default if unspecified) or Binary
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
  writer.AddRow(std::vector<double> {3, face[0], face[1], face[2]});
}

// Close the file
writer.Close();

```

See other samples the samples folder

## TODO

- More tests and samples
- Better encapsulated some method classes

## Author

Manlio Barajas
