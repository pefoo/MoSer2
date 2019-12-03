#ifndef ANYHELPER_H
#define ANYHELPER_H

#include <any>
#include <stdexcept>
#include <string>

namespace utility::datastructure {

std::string PrimitiveAnyToString(const std::any& data);
}  // namespace utility::datastructure

#endif  // ANYHELPER_H
