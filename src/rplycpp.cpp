#include "rplycpp.hpp"
#include "rply.h"
#include <iostream>

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
