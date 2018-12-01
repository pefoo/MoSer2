#ifndef SETTING_H
#define SETTING_H

#include <functional>
#include <string>
#include <utility>
#include "settingsprovider/converters.hpp"
#include "settingsprovider/isetting.hpp"
#include "settingsprovider/verifiers.hpp"

namespace settingsprovider {
///
/// \brief The setting implementation
///
template <typename ValueType>
class SettingBase : public ISetting<ValueType> {
 public:
  using Verifier = Verifier_t<ValueType>;
  using Converter = Converter_t<ValueType>;

  ///
  /// \brief SettingBase
  /// \param key  The key of the setting
  /// \param section  The section of the setting
  /// \param default_value  The default value for the setting
  /// \param verifier The verifier to apply when setting the value
  /// \param converter  The converter when the setting is created from a string
  ///
  SettingBase(std::string key, std::string section,
              const ValueType& default_value, const Verifier& verifier,
              const Converter& converter)
      : key_(std::move(key)),
        section_(std::move(section)),
        value_(default_value),
        verifier_(verifier),
        string_converter_(converter) {}

  ~SettingBase() override {}

  ///
  /// \copydoc ISetting::key()
  ///
  std::string key() const override { return this->key_; }

  ///
  /// \copydoc ISetting::section()
  ///
  std::string section() const override { return this->section_; }

  ///
  /// \copydoc ISetting::value()
  ///
  ValueType value() const override { return this->value_; }

  ///
  /// \brief Get the underlying type
  /// \return The underlying type
  ///
  ISettingIdentifier::Type type() const override {
    if (std::is_same<ValueType, int>::value) {
      return ISettingIdentifier::Type::INT;
    } else if (std::is_same<ValueType, std::string>::value) {
      return ISettingIdentifier::Type::STRING;
    }
  }

  ///
  /// \brief Set the value of the setting
  /// \param value The new value
  /// \param msg A pointer to a string, filled in case the value is not valid
  /// \return True, if the new value passes the verifier
  ///
  bool set_value(const std::string value, std::string* msg) {
    try {
      return this->set_value_internal(this->string_converter_(value), msg);
    } catch (...) {
      *msg = "Failed to convert the value.";
      return false;
    }
  }

 private:
  bool set_value_internal(const ValueType& value, std::string* msg) {
    if (this->verifier_(value, msg)) {
      this->value_ = value;
      return true;
    }
    return false;
  }
  std::string key_;
  std::string section_;
  ValueType value_;
  Verifier verifier_;
  Converter string_converter_;
};

///
/// \brief The default Setting class
///
template <typename ValueType>
class Setting : public SettingBase<ValueType> {};

///
/// \brief Specialized setting class for string types
///
template <>
class Setting<std::string> : public SettingBase<std::string> {
 public:
  Setting(std::string key, std::string section,
          const std::string& default_value = "",
          const Verifier& verifier = Verifiers<std::string>::default_verifier)
      : SettingBase(key, section, default_value, verifier,
                    Converters<std::string>::converter) {}
};

///
/// \brief Specialized setting class for int types
///
template <>
class Setting<int> : public SettingBase<int> {
 public:
  Setting(std::string key, std::string section, const int& default_value = 0,
          const Verifier& verifier = Verifiers<int>::default_verifier)
      : SettingBase(key, section, default_value, verifier,
                    Converters<int>::converter) {}
};

}  // namespace settingsprovider

#endif  // SETTING_H
