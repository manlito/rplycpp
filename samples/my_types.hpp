#ifndef MY_TYPES_HPP
#define MY_TYPES_HPP

// This is your sample application logic

#include <vector>
#include <iostream>

namespace myproject {

struct PointXYZ {
  float x;
  float y;
  float z;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

struct PointXYZRGB : PointXYZ {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

struct PointXYZRGBNormal : PointXYZRGB {
  float nx;
  float ny;
  float nz;
};


typedef std::vector<size_t> Face;

template<typename T>
struct Cloud {
  std::vector<T> points;
};

template<typename T>
struct Surface : Cloud<T> {
  std::vector<Face> faces;
};

std::ostream& operator<<(std::ostream &os, const PointXYZ &my_point) {
  os << "Point: [ "
     << my_point.x << " "
     << my_point.y << " "
     << my_point.z << " ]"
     << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream &os, const PointXYZRGB &my_point) {
  os << static_cast<PointXYZ>(my_point)
     << "Color: [ "
     << static_cast<size_t>(my_point.red) << " "
     << static_cast<size_t>(my_point.green) << " "
     << static_cast<size_t>(my_point.blue) << " ]"
     << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream &os, const PointXYZRGBNormal &my_point) {
  os << static_cast<PointXYZRGB>(my_point)
     << "Normal: [ "
     << my_point.nx << " "
     << my_point.ny << " "
     << my_point.nz << " ]"
     << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream &os, const Face &my_face) {
  os << "Face: [ ";
  for (const auto &vertex_index : my_face) {
    os << vertex_index << " ";
  }
  os << "]" << std::endl;
  return os;
}

}
#endif // MY_TYPES
