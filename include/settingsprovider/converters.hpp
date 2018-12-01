#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <functional>
#include <string>

namespace settingsprovider {
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

///
/// \brief The int converter
///
template <>
struct Converters<int> {
  static const Converter_t<int> converter;
};

}  // namespace settingsprovider

#endif  // CONVERTERS_H
