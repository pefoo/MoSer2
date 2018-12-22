#ifndef SETTINGVERIFIER_H
#define SETTINGVERIFIER_H

#include <functional>
#include <regex>
#include <string>

namespace settingsprovider {

using Verifier = std::function<bool(const std::string&)>;

///
/// \brief The default verifier. Always returns true.
///
constexpr bool DefaultVerifier([[gnu::unused]] const std::string& v) {
  return true;
}

template <typename T>
struct TypeVerifier;

template <>
struct TypeVerifier<int> {
  static bool VerifierFunc(const std::string& v) {
    std::regex rgx("\\d+");
    return std::regex_match(v, rgx);
  }
};

template <>
struct TypeVerifier<double> {
  static bool VerifierFunc(const std::string& v) {
    std::regex rgx("\\d+\\.\\d+");
    return std::regex_match(v, rgx);
  }
};

}  // namespace settingsprovider

#endif  // SETTINGVERIFIER_H
