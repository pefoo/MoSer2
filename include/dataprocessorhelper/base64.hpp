#ifndef BASE64_H
#define BASE64_H
#include <string>

namespace dataprocessorhelper {

///
/// \brief Encode a string
/// \param The string to encode
/// \return The base 64 representation
///
std::string EncodeBase64(const std::string& v);

}  // namespace dataprocessorhelper

#endif  // BASE64_H
