#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <string>
#include <functional>

namespace settingsprovider{
template <typename ValueType>
using Converter_t = std::function<ValueType(const std::string&)>;

///
/// \brief Converts struct
/// \details This is the undefined version
///
template <typename ValueType>
struct Converters;

///
/// \brief The string converter
///
template <>
struct Converters<std::string> {
  static const Converter_t<std::string> converter;
};
const Converter_t<std::string>
    Converters<std::string>::converter =
        [](const std::string& s) -> std::string { return s; };

///
/// \brief The int converter
///
template <>
struct Converters<int> {
  static const Converter_t<int> converter;
};
const Converter_t<int> Converters<int>::converter =
    [](const std::string& s) -> int { return std::stoi(s); };


} // namespace settingsprovider

#endif //CONVERTERS_H
