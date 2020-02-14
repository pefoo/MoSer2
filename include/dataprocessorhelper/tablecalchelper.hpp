#ifndef TABLECALCHELPER_H
#define TABLECALCHELPER_H

#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>
#include "utility/datastructure/table.hpp"

namespace dataprocessorhelper {

using ColumnFilter = std::function<bool(const std::string&)>;

/*
 * Helper for filter functions
 */
namespace Filter {
///
/// \brief The default filter. Removes timestamp columns, nothing else
///
static const ColumnFilter DefaultFilter = [](const std::string& col) {
  return col != "timestamp";
};

///
/// \brief A non filtering filter
///
static const ColumnFilter NoFilter =
    []([[gnu::unused]] const std::string& col) { return true; };

///
/// \brief Build a new filter
/// \note This one still ignores the timestamp column!
/// \param ignored_columns The columns to ignore
/// \return A new filter
///
inline ColumnFilter GetFilter(std::vector<std::string> ignored_columns) {
  return [ignored_columns](const std::string& col) {
    return DefaultFilter(col) &&
           (std::find(ignored_columns.begin(), ignored_columns.end(), col) ==
            ignored_columns.end());
  };
}

inline ColumnFilter GetInclusiveFilter(std::vector<std::string> columns) {
  return [columns](const std::string& col) {
    return DefaultFilter(col) &&
           (std::find(columns.begin(), columns.end(), col) != columns.end());
  };
}

}  // namespace Filter

/*
 * Helper and some binary operation implementations
 */
namespace {
std::vector<std::string> ApplyColumnFilter(
    const utility::datastructure::Table& data, ColumnFilter filter) {
  auto names = data.GetColumnNames();
  names.erase(std::remove_if(names.begin(), names.end(), std::not1(filter)),
              names.end());
  return names;
}

}  // namespace

///
/// \brief A wrapper for binary operations
/// \param data The data to execute the operation on
/// \param operation The operation to execute
/// \param init The inital value to use
/// \param filter The column filter to apply
///
template <typename ColumnType, typename BinaryOperation>
ColumnType AccumulateWrapper(const utility::datastructure::Table& data,
                             BinaryOperation operation,
                             ColumnType init = ColumnType{},
                             ColumnFilter filter = Filter::DefaultFilter) {
  static_assert(
      std::is_arithmetic<ColumnType>::value,
      "The accumulate function can only be used for arithmetic types.");
  for (const auto& c : ApplyColumnFilter(data, filter)) {
    auto column = data.GetDataColumn<ColumnType>(c).data();
    init = std::accumulate(column.begin(), column.end(), init, operation);
  }
  return init;
}

///
/// \brief Get the min value across possibly multiple columns
/// \param data The data table
/// \param filter A column filter
/// \return The minimum value across all selected columns
///
template <typename ColumnType>
ColumnType Min(const utility::datastructure::Table& data,
               ColumnFilter filter = Filter::DefaultFilter) {
  const ColumnType& (*func)(const ColumnType&, const ColumnType&) =
      std::min<ColumnType>;
  return AccumulateWrapper(data, func, std::numeric_limits<ColumnType>::max(),
                           filter);
}

///
/// \brief Get the max value across possibly multiple columns
/// \param data The data table
/// \param filter A column filter
/// \return The miximum value across all selected columns
///
template <typename ColumnType>
ColumnType Max(const utility::datastructure::Table& data,
               ColumnFilter filter = Filter::DefaultFilter) {
  const ColumnType& (*func)(const ColumnType&, const ColumnType&) =
      std::max<ColumnType>;
  return AccumulateWrapper(data, func, std::numeric_limits<ColumnType>::min(),
                           filter);
}

///
/// \brief Get the avg value across possibly multiple columns
/// \param data The data table
/// \param filter A column filter
/// \return The average value across all selected columns
///
template <typename ColumnType>
double Avg(const utility::datastructure::Table& data,
           ColumnFilter filter = Filter::DefaultFilter) {
  double sum = AccumulateWrapper<ColumnType>(
      data, [](const ColumnType& a, const ColumnType& b) { return a + b; }, 0,
      filter);
  return sum / (ApplyColumnFilter(data, filter).size() * data.MaxSize());
}

}  // namespace dataprocessorhelper

#endif  // TABLECALCHELPER_H
