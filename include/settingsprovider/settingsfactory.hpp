#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "isettingsprovider.hpp"
#include "settingidentifier.hpp"
#include "settingverifier.hpp"

namespace settingsprovider {

///
/// \brief The settings factory
/// \details The factory creates a new settings provider for each call to
/// SettingsFactory::ReadFromFile. Thus actual registration is delayed to this
/// call. Once registered settings are carried over to subsequent calls.
///
class SettingsFactory {
 public:
  ///
  /// \brief Register a setting
  /// \param key The setting key
  /// \param section The setting section
  /// \param default_value The default value
  /// \param verifier The verifier to use
  ///
  void RegisterSetting(const std::string& key, const std::string& section,
                       const std::string& default_value = "",
                       const Verifier& verifier = DefaultVerifier);

  ///
  /// \brief Register a setting
  /// \param identifier The identifier of the setting
  /// \param default_value The default value
  /// \param verifier The verifier to use
  ///
  void RegisterSetting(const SettingIdentifier& identifier,
                       const std::string& default_value = "",
                       const Verifier& verifier = DefaultVerifier);

  ///
  /// \brief Read settings from file
  /// \param file The file to read
  /// \param msg The message buffer
  /// \return A new SettingsProvider
  ///
  std::unique_ptr<ISettingsProvider> ReadFromFile(
      const std::string& file, std::vector<std::string>* msg);

 private:
  struct Setting {
    Setting() = default;
    Setting(std::string key, std::string section,
            std::string default_value = "", Verifier verifier = DefaultVerifier)
        : key_(std::move(key)),
          section_(std::move(section)),
          value_(std::move(default_value)),
          verifier_(std::move(verifier)) {}
    std::string key_ = "";
    std::string section_ = "";
    std::string value_ = "";
    Verifier verifier_ = DefaultVerifier;
  };
  std::vector<Setting> settings_;
};
}  // namespace settingsprovider

#endif  // SETTINGSFACTORY_H
