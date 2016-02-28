#include "rplycpp.hpp"
#include "rply.h"

using namespace rplycpp;

int ply_read_vertex_or_face_cb(p_ply_argument argument) {
  // Check if is a vertex or face
  long length;
  ply_get_argument_property(argument, nullptr, &length, nullptr);
  bool is_face = length > 1;

  // Parse input argument and grab PLY object
  void *reader;
  ply_get_argument_user_data(argument, &reader, nullptr);
  PLYReader* ply_reader = static_cast<PLYReader*>(reader);
  PLYElement &ply_element = ply_reader->GetCurrentElement();

  // Estimate row size, for list (face) or scalar
  size_t row_size = 0;
  if (is_face) {
    row_size = length + 1;
  } else {
    row_size = ply_element.GetProperties().size();
  }

  // Get the current index, relative to all points in the element
  size_t current_value_index = ply_element.GetCurrentIndex();
  size_t current_value_offset = current_value_index % row_size;
  double value = ply_get_argument_value(argument);

  // If it is the first element in the row, clear previous temporal value store
  if (current_value_offset == 0) {
    ply_element.ClearValues();
  }
  ply_element.AddValue(value);
  ply_element.IncrementIndex();

  // If it is the last column, invoke the handler
  if (current_value_offset == row_size - 1) {
    ply_reader->GetCurrentHandler()(ply_element.GetValues());
  }

  // When the last element, go the the next element
  if (current_value_index >= ply_element.GetTotalInstances() * row_size - 1) {
    ply_reader->IncrementElement();
  }

  return 1;
}

bool PLYReader::Open(std::string filename)
{
  ply_file_ = ply_open(filename.c_str(), nullptr, 0, nullptr);

  // Check if it could be opened
  if (!ply_file_) return false;

  // Flag used to tell if we can call Close method
  file_open_ = true;

  // Parse header
  if (!ply_read_header(static_cast<p_ply>(ply_file_))) return false;

  // Get elements
  p_ply_element element = NULL;
  while ((element = ply_get_next_element(static_cast<p_ply>(ply_file_), element))) {
    PLYElement ply_element;

    // Get element info
    const char *element_name;
    long ninstances = 0;
    ply_get_element_info(element, &element_name, &ninstances);
    ply_element.SetTotalInstances(ninstances);
    ply_element.SetName(element_name);

    // iterate over all properties of current element
    p_ply_property property = NULL;
    while ((property = ply_get_next_property(element, property))) {
        const char *property_name;
        e_ply_type type, length_type, value_type;
        ply_get_property_info(property, &property_name, &type, &length_type, &value_type);
        if (!ply_set_read_cb(static_cast<p_ply>(ply_file_),
                             element_name,
                             property_name,
                             ply_read_vertex_or_face_cb,
                             this, 0)) return false;
        PLYProperty property;
        property.name = property_name;
        switch (type) {
          case PLY_INT8: property.type = PLYDataType::PLY_INT8; break;
          case PLY_UINT8: property.type = PLYDataType::PLY_UINT8; break;
          case PLY_INT16: property.type = PLYDataType::PLY_INT16; break;
          case PLY_UINT16: property.type = PLYDataType::PLY_UINT16; break;
          case PLY_INT32: property.type = PLYDataType::PLY_INT32; break;
          case PLY_UIN32: property.type = PLYDataType::PLY_UINT32; break;
          case PLY_FLOAT32: property.type = PLYDataType::PLY_FLOAT32; break;
          case PLY_FLOAT64: property.type = PLYDataType::PLY_FLOAT64; break;
          case PLY_CHAR: property.type = PLYDataType::PLY_CHAR; break;
          case PLY_UCHAR: property.type = PLYDataType::PLY_UCHAR; break;
          case PLY_SHORT: property.type = PLYDataType::PLY_SHORT; break;
          case PLY_USHORT: property.type = PLYDataType::PLY_USHORT; break;
          case PLY_INT: property.type = PLYDataType::PLY_INT; break;
          case PLY_UINT: property.type = PLYDataType::PLY_UINT; break;
          case PLY_FLOAT: property.type = PLYDataType::PLY_FLOAT; break;
          case PLY_DOUBLE: property.type = PLYDataType::PLY_DOUBLE; break;
          case PLY_LIST: property.type = PLYDataType::PLY_LIST; break;
        }
        ply_element.AddProperty(property);
    }
    elements_.push_back(ply_element);
  }
  return true;
}

bool PLYReader::Read(std::vector<std::function<void (const std::vector<double>&)>> handlers)
{
  handlers_ = handlers;
  if (!ply_read(static_cast<p_ply>(ply_file_))) return false;

  return true;
}

bool PLYReader::Close()
{
  if (file_open_) {
    if (!ply_close(static_cast<p_ply>(ply_file_))) return false;
    file_open_ = false;
  }

  return true;
}
