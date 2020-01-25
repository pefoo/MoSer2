#ifndef TABLEFILTER_H
#define TABLEFILTER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace utility {
namespace datastructure {

/*
 * Filter always act like a blacklist.
 * A filter that always returns true will filter EVERYTHING.
 */

// A column name based filter
using ColumnFilter = std::function<bool(const std::string&)>;
// A field value based filter. The column name and the field value (as string)
// is passed
using FieldFilter = std::function<bool(const std::string&, const std::string&)>;

constexpr bool NoColumnFilter([[maybe_unused]] const std::string&) {
  return false;
}
constexpr bool NoFieldFilter([[maybe_unused]] const std::string&,
                             [[maybe_unused]] const std::string&) {
  return false;
}

///
/// \brief Create a new column filter
/// \param blacklist The columns to ignore
/// \return A new column filter
///
ColumnFilter CreateColumnFilter(const std::vector<std::string>& blacklist);

///
/// \brief Create a new field filter
/// \param blacklist The column / value pairs to ignore
/// \return A new field filter
///
FieldFilter CreateFieldFilter(
    const std::multimap<std::string, std::string>& blacklist);
}  // namespace datastructure

}  // namespace utility

#endif  // TABLEFILTER_H
