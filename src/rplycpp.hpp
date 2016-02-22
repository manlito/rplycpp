#ifndef RPLYCPP_HPP
#define RPLYCPP_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>

namespace rplycpp {

// Signature of the handler. Note all values are double, since that is how
// we get them from the
typedef std::function<void (const std::vector<double>&)> PLYReadHandler;

struct PLYProperty {
  std::string name;
};

class PLYElement {
public:
  PLYElement() : current_index_(0) {}

  // Properties
  void AddProperty(PLYProperty property)  { properties_.push_back(property); }
  const std::vector<PLYProperty>& GetProperties() const { return properties_; }

  // Number of vertices/faces
  void SetTotalInstances(size_t total_instances) { total_instances_ = total_instances; }
  size_t GetTotalInstances() const { return total_instances_; }

  // Current value index accesor
  void IncrementIndex() { ++current_index_; }
  size_t GetCurrentIndex() { return current_index_; }

  // Add a value to the current row. Are public because they're used in callback
  void AddValue(double value) { row_values_.push_back(value); }
  std::vector<double> GetValues() { return row_values_; }
  void ClearValues() { row_values_.clear(); }

protected:
  size_t current_index_;
  size_t total_instances_;
  std::vector<PLYProperty> properties_;
  // Keep track of the current element property read
  std::vector<double> row_values_;
};

class PLYReader {
public:
  PLYReader() : current_element_(0) {}
  bool Open(std::string filename);
  bool Read(std::vector<std::function<void (const std::vector<double>&)>> handlers);

  const std::vector<PLYElement>& GetElements() const
  { return elements_; }
  PLYElement& GetCurrentElement()
  { return elements_[current_element_]; }
  PLYReadHandler& GetCurrentHandler()
  { return handlers_[current_element_]; }

  // Current value index accesor
  void IncrementElement() { ++current_element_; }

protected:
  // Original rply object used for reading
  void *ply_file_;
  std::vector<PLYElement> elements_;
  // Keep track of the current element
  size_t current_element_;
  // Interfaces with the reader
  std::vector<PLYReadHandler> handlers_;
};

}

#endif // RPLYCPP_HPP
