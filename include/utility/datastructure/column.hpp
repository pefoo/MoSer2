#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <string>

namespace utility {
namespace datastructure {

///
/// \brief A table column
///
class Column {
 public:
  ///
  /// \brief Create a new instance
  /// \param name The column name
  ///
  explicit Column(std::string name) : name_(std::move(name)) {}

  virtual ~Column() = default;

  ///
  /// \brief Get the column name
  /// \return The name of the column
  ///
  [[nodiscard]] std::string name() const { return this->name_; }

  ///
  /// \brief Set the column name
  /// \param name The new name of the column
  ///
  void set_name(const std::string& name) { this->name_ = name; }

  ///
  /// \brief Get the column size
  /// \return The column size
  ///
  [[nodiscard]] virtual size_t size() const = 0;

  ///
  /// \brief Get the string representation of an element
  /// \param index The index of the element
  /// \return The string representation of the element
  ///
  [[nodiscard]] virtual std::string ElementAtToString(
      const size_t index) const = 0;

  ///
  /// \brief Clone the underlying data
  /// \return A clone of the underlying data
  /// \details Use this to get a deep clone of the actual data object. This
  /// method is implemented in utility::datastructure::DataColumn and invokes
  /// the copy constructur of said implementation.
  ///
  [[nodiscard]] virtual Column* Clone() const = 0;

 private:
  std::string name_;
};

}  // namespace datastructure
}  // namespace utility

#endif  // COLUMN_HPP
