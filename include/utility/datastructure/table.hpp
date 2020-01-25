#ifndef TABLE_HPP
#define TABLE_HPP

#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "functional"
#include "utility/datastructure/column.hpp"
#include "utility/datastructure/datacolumn.hpp"
#include "utility/datastructure/tablefilter.hpp"

namespace utility {
namespace datastructure {

///
/// \brief The Table class
///
class Table {
 public:
  using ColumnRsc = std::unique_ptr<Column>;

  ///
  /// \brief Create a new instance
  /// \param name The table name
  ///
  explicit Table(std::string name = "default") : name_(std::move(name)) {}

  ///
  /// \brief Copy constructor
  ///
  Table(const Table& o) : name_(o.name()) {
    for (const auto& c : o.columns_) {
      this->columns_.push_back(std::unique_ptr<Column>(c->Clone()));
    }
  }

  ///
  /// \brief Move constructor
  ///
  Table(Table&& o) noexcept
      : name_(std::exchange(o.name_, "")), columns_(std::move(o.columns_)) {}

  ///
  /// \brief Coppy assign
  ///
  Table& operator=(const Table& o) { return *this = Table{o}; }

  ///
  /// \brief Move assign
  ///
  Table& operator=(Table&& o) noexcept {
    this->name_ = std::move(o.name_);
    this->columns_ = std::move(o.columns_);
    return *this;
  }

  ///
  /// \brief Add a value to a column. If the column does not exist, a new one is
  /// created
  /// \param value The value to add
  /// \param column The column to add to
  /// \tparam ColumnType The type of the column
  ///
  template <typename ColumnType>
  void AddValue(const ColumnType& value, const std::string& column) {
    DataColumn<ColumnType>& dc = this->GetDataColumn<ColumnType>(column, true);
    dc.data().push_back(value);
  }

  ///
  /// \brief Add a new column
  /// \param column The column to add
  ///
  template <typename ColumnType>
  void AddColumn(DataColumn<ColumnType> column) {
    this->columns_.push_back(
        std::make_unique<DataColumn<ColumnType>>(std::move(column)));
  }

  ///
  /// \brief Get a column
  /// \param name The name of the column to get
  /// \param implicit_create_column True, if a new column should be created in
  /// case the column does not exist
  /// \return The underlying data column
  ///
  template <typename ColumnType>
  DataColumn<ColumnType>& GetDataColumn(const std::string& name,
                                        bool implicit_create_column = false) {
    if (auto dc = FindColumn<ColumnType>(name)) {
      return *dc;
    }

    if (implicit_create_column) {
      this->columns_.push_back(std::make_unique<DataColumn<ColumnType>>(
          DataColumn<ColumnType>{name}));
      return *dynamic_cast<DataColumn<ColumnType>*>(
          this->columns_.back().get());
    }
    throw std::runtime_error("No such column " + name + " in table " +
                             this->name_);
  }

  ///
  /// \brief Get a column
  /// \param name The name of the column to get
  /// \return The underlying data column
  ///
  template <typename ColumnType>
  [[nodiscard]] DataColumn<ColumnType>& GetDataColumn(
      const std::string& name) const {
    if (auto dc = FindColumn<ColumnType>(name)) {
      return *dc;
    }
    throw std::runtime_error("No such column " + name + " in table " +
                             this->name_);
  }

  ///
  /// \brief Get all column names
  /// \return A vector with all column names
  ///
  [[nodiscard]] std::vector<std::string> GetColumnNames() const {
    std::vector<std::string> names;
    std::transform(this->columns_.begin(), this->columns_.end(),
                   std::back_inserter(names),
                   [](const ColumnRsc& x) { return x->name(); });
    return names;
  }

  // TODO: Rewrite (this and everyone that calls it) to use filter as blacklist
  ///
  /// \brief Write the table content to file
  /// \param separator The column field separator
  /// \param file The file to write to (a tmp file is choosen if left empty)
  /// \param write_header True, if the first line should contain the column
  /// names
  /// \param column_filter A column name filter to apply (not matching columns
  /// wont be written to the file
  /// \param field_filter A filter to apply to the fields. The current column
  /// name and the field value (as string) is passed to the expression
  /// \return The path to the written file
  /// \details If the table contains a column that
  /// is called timestamp, this column will be written to the first column of
  /// the file. This is done to ensure compatibility with existing gpl scripts
  ///
  std::string ToFile(const char separator, const std::string& file = "",
                     bool write_header = true,
                     const ColumnFilter& column_filter = NoColumnFilter,
                     const FieldFilter field_filter = NoFieldFilter) const {
    std::string out_file = file;
    if (out_file.empty()) {
      char name_template[] = "/tmp/table_XXXXXX";
      mkstemp(name_template);
      out_file = name_template;
    }
    size_t max_size = this->MaxSize();
    std::ofstream out{out_file};
    std::stringstream line_buffer{};

    // columns provides a sorted view to the stored data.
    // This ensures, that the timestamp column is always at positon 0
    const std::string timestamp_column = "timestamp";
    std::vector<Column*> columns;
    bool first_reserved = false;
    if (!column_filter(timestamp_column) &&
        std::any_of(this->columns_.begin(), this->columns_.end(),
                    [timestamp_column](const ColumnRsc& c) {
                      return c->name() == timestamp_column;
                    })) {
      // Reserve the first element
      columns.push_back(nullptr);
      first_reserved = true;
    }
    for (const auto& c : this->columns_) {
      if (first_reserved && c->name() == timestamp_column) {
        columns[0] = c.get();
      } else {
        columns.push_back(c.get());
      }
    }

    if (write_header) {
      for (const auto& c : columns) {
        if (column_filter(c->name())) continue;
        out << c->name() << separator;
      }
      // remove trailing separator
      out.seekp(-1, std::ios_base::end);
      out << std::endl;
    }

    for (size_t i = 0; i < max_size; ++i) {
      // reset the line buffer
      line_buffer.str(std::string{});
      line_buffer.clear();
      bool drop_line{false};
      for (const auto& c : columns) {
        // drop column
        if (column_filter(c->name())) continue;
        std::string value = c->ElementAtToString(i);
        // drop entire line
        if (field_filter(c->name(), value)) {
          drop_line = true;
          break;
        }
        if (c->size() > i) {
          line_buffer << value << separator;
        } else {
          line_buffer << "N.a." << separator;
        }
      }
      if (!drop_line) {
        // remove trailing separator
        line_buffer.seekp(-1, std::ios_base::end);
        line_buffer << std::endl;
        out << line_buffer.str();
      }
    }
    return out_file;
  }

  ///
  /// \brief Check if all columns are of same size
  /// \return True, if all columns have the same size
  ///
  [[nodiscard]] bool IsUniformSize() const {
    size_t s = this->columns_.front()->size();
    for (const auto& c : this->columns_) {
      if (s != c->size()) return false;
    }
    return true;
  }

  ///
  /// \brief Get the size of the largest column
  /// \return The size of the largest column
  ///
  [[nodiscard]] size_t MaxSize() const {
    size_t max_size = 0;
    for (const auto& c : this->columns_) {
      max_size = std::max(max_size, c->size());
    }
    return max_size;
  }

  ///
  /// \brief Get the column count
  /// \return The coloumn count of the table
  ///
  [[nodiscard]] size_t ColumnCount() const { return this->columns_.size(); }

  ///
  /// \brief Get the table name
  /// \return The name of the table
  ///
  [[nodiscard]] std::string name() const { return this->name_; }

  ///
  /// \brief Set the table name
  /// \param name The name of the table
  ///
  void set_name(const std::string& name) { this->name_ = name; }

 private:
  template <typename ColumnType>
  [[nodiscard]] DataColumn<ColumnType>* FindColumn(
      const std::string& name) const {
    for (auto& c : this->columns_) {
      if (c->name() == name) {
        auto dc = dynamic_cast<DataColumn<ColumnType>*>(c.get());
        if (!dc) {
          // GCC gives mangled type names but what ever, better than nothing
          throw std::runtime_error("The column " + name + " is not of type " +
                                   std::string{typeid(ColumnType).name()});
        }
        return dc;
      }
    }
    return nullptr;
  }
  std::string name_;
  std::vector<ColumnRsc> columns_;
};

}  // namespace datastructure
}  // namespace utility

#endif  // TABLE_HPP
