#include "utility/datastructure/tablefilter.hpp"

utility::datastructure::ColumnFilter utility::datastructure::CreateColumnFilter(
    const std::vector<std::string> &blacklist) {
  return [blacklist](const std::string &column) {
    return std::any_of(blacklist.begin(), blacklist.end(),
                       [column](const std::string &c) { return c == column; });
  };
}

utility::datastructure::FieldFilter utility::datastructure::CreateFieldFilter(
    const std::multimap<std::string, std::string> &blacklist) {
  return [blacklist](const std::string &c, const std::string &v) {
    for (auto const &[key, value] : blacklist) {
      if (key == c && value == v) return true;
    }
    return false;
  };
}
