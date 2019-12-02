#ifndef DATACOLUMN_HPP
#define DATACOLUMN_HPP

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "utility/datastructure/column.hpp"

namespace utility {
namespace datastructure {

///
/// \brief The DataColumn class
/// \tparam ColumnType The type of the column
///
template <typename ColumnType>
class DataColumn : public Column {
  static_assert (!std::is_pointer<ColumnType>::value, "Pointer fields are not supported.");
 public:
  ///
  /// \brief Create a new instance
  /// \param name The name of the column
  /// \param data The initial data for the column
  ///
  DataColumn(std::string name, std::vector<ColumnType> data = {})
      : Column(std::move(name)), values_(std::move(data)) {}

  ///
  /// \brief Get the underlying data vector
  /// \return The data vector
  ///
  std::vector<ColumnType>& data() { return this->values_; }

  ///
  /// \copydoc utility::datastructure::Column::size
  ///
  size_t size() const override { return this->values_.size(); }

  ///
  /// \copydoc utility::datastructure::Column::ElementAtToString
  ///
  std::string ElementAtToString(const size_t index) const override {
    if (this->size() > index) {
      return this->StringConverter(index);
    }
    throw std::out_of_range("Index " + std::to_string(index) +
                            " is out of range for column " + this->name());
  }

  ///
  /// \copydoc utility::datastructure::Column::Clone
  ///
  Column* Clone() const override {
    return new DataColumn<ColumnType>{*this};
  }

 private:
  inline std::string StringConverter(const size_t index) const;
  std::vector<ColumnType> values_;
};

template <typename ColumnType>
inline std::string DataColumn<ColumnType>::StringConverter(const size_t index) const {
  return std::to_string(this->values_.at(index));
}
template <>
inline std::string DataColumn<std::string>::StringConverter(const size_t index) const {
  return this->values_.at(index);
}

}  // namespace datastructure
}  // namespace utility

#endif  // DATACOLUMN_HPP
