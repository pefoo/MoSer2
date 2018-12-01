#ifndef VERIFIERS_H
#define VERIFIERS_H

#include <functional>
#include <string>

namespace settingsprovider {

template <typename ValueType>
using Verifier_t = std::function<bool(const ValueType&, std::string*)>;

///
/// \brief Verifiers struct
/// \details Provides the default verifier, that always returns true
///
template <typename ValueType>
struct Verifiers {
  static const Verifier_t<ValueType> default_verifier;
};
template <typename ValueType>
const Verifier_t<ValueType> Verifiers<ValueType>::default_verifier =
    [](const ValueType&, std::string*) -> bool { return true; };

}  // namespace settingsprovider
#endif  // VERIFIERS_H
