#ifndef SETTINGVERIFIER_H
#define SETTINGVERIFIER_H

#include <filesystem>
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

static bool FileExistVerifier(const std::string& v) {
  return std::filesystem::exists(v);
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
    std::regex rgx(R"(\d+\.\d+)");
    return std::regex_match(v, rgx);
  }
};

}  // namespace settingsprovider

#endif  // SETTINGVERIFIER_H
