#ifndef RPLYCPP_HPP
#define RPLYCPP_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>

namespace rplycpp {

// Signature of the handler. Note all values are double, since that is how
// we get them from RPLy library
typedef std::function<void (const std::vector<double>&)> PLYReadHandler;
const PLYReadHandler DEFAULT_HANDLER = [](const std::vector<double> &vertex) {};

enum class PLYDataType {
  PLY_INT8, PLY_UINT8, PLY_INT16, PLY_UINT16,
  PLY_INT32, PLY_UINT32, PLY_FLOAT32, PLY_FLOAT64,
  PLY_CHAR, PLY_UCHAR, PLY_SHORT, PLY_USHORT,
  PLY_INT, PLY_UINT, PLY_FLOAT, PLY_DOUBLE,
  PLY_LIST
};

struct PLYProperty {
  std::string name;
  PLYDataType type;
  // Values used only for list writing
  PLYDataType list_length_type;
  PLYDataType list_value_type;
  size_t list_length;
  PLYProperty() :
    name("UNKNOWN"),
    type(PLYDataType::PLY_FLOAT),
    list_length_type(PLYDataType::PLY_UCHAR),
    list_value_type(PLYDataType::PLY_INT) {}
};

class PLYElement {
public:
  PLYElement() : current_index_(0) {}

  // Properties
  void SetName(std::string name) { name_ = name; }
  std::string GetName() const { return name_; }
  void AddProperty(PLYProperty property)  { properties_.push_back(property); }
  const std::vector<PLYProperty>& GetProperties() const { return properties_; }

  // Number of vertices/faces
  void SetTotalInstances(size_t total_instances) { total_instances_ = total_instances; }
  size_t GetTotalInstances() const { return total_instances_; }

  // Current value index accesor
  void IncrementIndex() { ++current_index_; }
  size_t GetCurrentIndex() const { return current_index_; }

  // Add a value to the current row. Are public because they're used in callback
  void AddValue(double value) { row_values_.push_back(value); }
  std::vector<double> GetValues() const { return row_values_; }
  void ClearValues() { row_values_.clear(); }

protected:
  size_t current_index_;
  size_t total_instances_;
  std::string name_;
  std::vector<PLYProperty> properties_;
  // Keep track of the current element property read
  std::vector<double> row_values_;
};

class PLYReader {
public:
  PLYReader() : current_element_(0), file_open_(false) {}
  bool Open(std::string filename);
  bool Read(std::vector<std::function<void (const std::vector<double>&)>> handlers);

  const std::vector<PLYElement>& GetElements() const
  { return elements_; }
  PLYElement& GetCurrentElement()
  { return elements_[current_element_]; }
  PLYReadHandler GetCurrentHandler()
  {
    if (current_element_ < handlers_.size())
      return handlers_[current_element_];
    return DEFAULT_HANDLER;
  }

  // Current value index accesor
  void IncrementElement() { ++current_element_; }
  bool Close();
  ~PLYReader() { Close(); }

protected:
  // Original rply object used for reading
  void *ply_file_;
  bool file_open_;
  std::vector<PLYElement> elements_;
  // Keep track of the current element
  size_t current_element_;
  // Interfaces with the reader
  std::vector<PLYReadHandler> handlers_;
};

enum class PLYStorageMode {
  PLY_BIG_ENDIAN, PLY_LITTLE_ENDIAN, PLY_ASCII, PLY_DEFAULT
};

class PLYWriter {
public:
  PLYWriter() : header_written_(false), file_open_(false) {}
  bool Open(std::string filename,
            PLYStorageMode storage_mode = PLYStorageMode::PLY_ASCII);

  bool AddElement(std::string name,
                  const std::vector<PLYProperty> &properties,
                  size_t total_rows);
  bool AddRow(const std::vector<double> &values);
  bool Close();
  ~PLYWriter() { Close(); }
protected:
  bool WriteHeader();
  // Original rply object used for writing
  void *ply_file_;
  bool header_written_;
  bool file_open_;
};
}

#endif // RPLYCPP_HPP
