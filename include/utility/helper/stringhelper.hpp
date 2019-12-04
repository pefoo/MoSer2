#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <regex>
#include <string>

namespace utility {
namespace helper {

///
/// \brief Check if string a starts with value
/// \param a The string to check
/// \param value The prefix
/// \return True, if string a starts with value
///
[[nodiscard]] bool StringStartsWith(const std::string& a,
                                    const std::string& value);

///
/// \brief Grep in string using regular expression
/// \param a The string to grep in
/// \param rgx The regular expression to grep
/// \param sm The match object
/// \return True if the regular expression matches.
///
bool StringRgxGrep(const std::string& a, const std::string& rgx,
                   std::smatch* sm = nullptr);

}  // namespace helper

}  // namespace utility

#endif  // STRINGHELPER_H
