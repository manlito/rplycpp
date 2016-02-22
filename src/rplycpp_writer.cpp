#include "rplycpp.hpp"
#include "rply.h"

namespace rplycpp {
  e_ply_type FromRPLYCPPToEply(rplycpp::PLYDataType type) {
    e_ply_type ply_type;
    switch (type) {
      case PLYDataType::PLY_INT8: ply_type = PLY_INT8; break;
      case PLYDataType::PLY_UINT8: ply_type = PLY_UINT8; break;
      case PLYDataType::PLY_INT16: ply_type = PLY_INT16; break;
      case PLYDataType::PLY_INT32: ply_type = PLY_INT32; break;
      case PLYDataType::PLY_UINT32: ply_type = PLY_UIN32; break;
      case PLYDataType::PLY_FLOAT32: ply_type = PLY_FLOAT32; break;
      case PLYDataType::PLY_FLOAT64: ply_type = PLY_FLOAT64; break;
      case PLYDataType::PLY_CHAR: ply_type = PLY_CHAR; break;
      case PLYDataType::PLY_UCHAR: ply_type = PLY_UCHAR; break;
      case PLYDataType::PLY_SHORT: ply_type = PLY_SHORT; break;
      case PLYDataType::PLY_USHORT: ply_type = PLY_USHORT; break;
      case PLYDataType::PLY_INT: ply_type = PLY_INT; break;
      case PLYDataType::PLY_UINT: ply_type = PLY_UINT; break;
      case PLYDataType::PLY_FLOAT: ply_type = PLY_FLOAT; break;
      case PLYDataType::PLY_DOUBLE: ply_type = PLY_DOUBLE; break;
      case PLYDataType::PLY_LIST: ply_type = PLY_LIST; break;
      default: ply_type = PLY_FLOAT; break;
    }
    return ply_type;
  }
}

using namespace rplycpp;
bool PLYWriter::Open(std::string filename, PLYStorageMode storage_mode)
{
  // Map storage mode
  e_ply_storage_mode ply_storage_mode = PLY_DEFAULT;
  switch(storage_mode) {
    case PLYStorageMode::PLY_BIG_ENDIAN:
      ply_storage_mode = PLY_BIG_ENDIAN; break;
    case PLYStorageMode::PLY_LITTLE_ENDIAN:
      ply_storage_mode = PLY_LITTLE_ENDIAN; break;
    case PLYStorageMode::PLY_ASCII:
      ply_storage_mode = PLY_ASCII; break;
    default:
      ply_storage_mode = PLY_DEFAULT; break;
  }

  ply_file_ = ply_create(filename.c_str(), ply_storage_mode, nullptr, 0, nullptr);

  // Check if it could be opened
  if (!ply_file_) return false;

  // Flag used to tell if we can call Close method
  file_open_ = true;

  return true;
}

bool PLYWriter::AddElement(std::string name,
                           const std::vector<PLYProperty> &properties,
                           size_t total_rows)
{
  if (!ply_add_element(static_cast<p_ply>(ply_file_),
                       name.c_str(),
                       total_rows)) return false;
  for (const PLYProperty &property : properties) {
    e_ply_type ply_type = FromRPLYCPPToEply(property.type);

    if (property.type != PLYDataType::PLY_LIST) {
      if (!ply_add_scalar_property(static_cast<p_ply>(ply_file_),
                                   property.name.c_str(),
                                   ply_type))
        return false;
    } else {
      e_ply_type ply_list_length_type = FromRPLYCPPToEply(property.list_length_type);
      e_ply_type ply_value_type = FromRPLYCPPToEply(property.list_value_type);
      if (!ply_add_list_property(static_cast<p_ply>(ply_file_),
                                 property.name.c_str(),
                                 ply_list_length_type,
                                 ply_value_type))
        return false;
    }
  }
  return true;
}

bool PLYWriter::WriteHeader()
{
  if (!header_written_) {
    if (!ply_write_header(static_cast<p_ply>(ply_file_)))
      return false;
    header_written_ = true;
  }
  return true;
}

bool PLYWriter::AddRow(const std::vector<double> &values)
{
  if (!WriteHeader()) return false;
  for (const double &value : values) {
    if (!ply_write(static_cast<p_ply>(ply_file_), value)) return false;
  }
  return true;
}

bool PLYWriter::Close()
{
  if (file_open_) {
    if (!WriteHeader()) return false;
    if (!ply_close(static_cast<p_ply>(ply_file_))) return false;
    file_open_ = false;
  }
  return true;
}
