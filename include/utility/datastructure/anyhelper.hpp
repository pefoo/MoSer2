#ifndef ANYHELPER_H
#define ANYHELPER_H

#include <string>
#include <any>
#include <stdexcept>

namespace utility::datastructure {

std::string PrimitiveAnyToString(const std::any& data);
} // namespace utility::datastructure

#endif //ANYHELPER_H
