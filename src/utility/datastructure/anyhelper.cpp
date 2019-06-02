#include "utility/datastructure/anyhelper.hpp"
#include <string>

std::string utility::datastructure::PrimitiveAnyToString(const std::any& data) {
  auto& type = data.type();
  if (type == typeid(int)) {
    return std::to_string(std::any_cast<int>(data));
  } if (type == typeid(int64_t)) {
    return std::to_string(std::any_cast<int64_t>(data));
  } else if (type == typeid(float)) {
    return std::to_string(std::any_cast<float>(data));
  } else if (type == typeid(double)) {
    return std::to_string(std::any_cast<double>(data));
  } else if (type == typeid(std::string)) {
    return std::any_cast<std::string>(data);
  } else if (type == typeid(bool)) {
    return std::to_string(std::any_cast<bool>(data));
  } else if (type == typeid(char)) {
    return std::to_string(std::any_cast<char>(data));
  }
  throw std::runtime_error("Not supported data type: " +
                           std::string(type.name()));
}
