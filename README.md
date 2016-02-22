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
#include "rplycpp/rply.cpp"

// Some randome data type from your app logic
MyStruct my_struct;

rplycpp::PLYReader reader
read_status = reader.Open("your_file.ply");

auto vertices_handler = [&my_struct](const std::vector<double> &vertex) {
  // This is your app logic, just remember there is an array of double with all row values
  MyPoint point;
  point.x = vertex[0]; 
  point.y = vertex[1];
  point.z = vertex[2];
  my_struct.points.push_back(point);
};
auto faces_handler = [&my_struct](const std::vector<double> &vertex_indexes) {
  // This is is algo your app logic
  MyFace face;
  face[0] = vertex_indexes[0]; 
  face[1] = vertex_indexes[1]; 
  face[2] = vertex_indexes[2]; 
  my_struct.faces.push_back(face);
};

// Register the handlers
std::vector<rplycpp::PLYReadHandler> handlers;
handlers.push_back(vertices_handler);
handlers.push_back(faces_handler);

// Read. This will be invoke your handlers when needed
read_status = reader.Read(handlers);
```
