#ifndef TEMPLATETOKEN_H
#define TEMPLATETOKEN_H

#include <functional>
#include <string>

namespace reporter {
namespace templateprocessor {

///
/// \brief A template token with lazy value evaluation
/// \details The token value is evaluated once the value() function is called.
/// If cache is activated, subsequent calls will use the cached value.
///
class TemplateToken {
 public:
  ///
  /// \brief Create new instance
  /// \param key The key of the token (the token to be replaced)
  /// \param value_func The function to generate the value of this token
  /// \param cache A value indicating whether to cache the value
  ///
  TemplateToken(std::string key, std::function<std::string()> value_func,
                bool cache = true);

  ///
  /// \brief Get the key
  /// \return The key
  ///
  [[nodiscard]] std::string key() const;

  ///
  /// \brief Get the value
  /// \return The value
  ///
  std::string value();

 private:
  std::string key_;
  std::string value_;
  std::function<std::string()> value_func_;
  bool cache_;
};
}  // namespace templateprocessor
}  // namespace reporter

#endif  // TEMPLATETOKEN_H
