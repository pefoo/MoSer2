#include "settingsprovider/converters.hpp"
#include <string>

namespace settingsprovider {

const Converter_t<std::string> Converters<std::string>::converter =
    [](const std::string& s) -> std::string { return s; };

const Converter_t<int> Converters<int>::converter =
    [](const std::string& s) -> int { return std::stoi(s); };

}  // namespace settingsprovider
