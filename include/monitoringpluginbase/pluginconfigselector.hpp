#ifndef PLUGINCONFIGSELECTOR_HPP
#define PLUGINCONFIGSELECTOR_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "imonitoringplugin/ipluginconfigselector.hpp"

namespace monitoringpluginbase {

///
/// \brief The PluginConfigSelector class
///
class PluginConfigSelector : public imonitorplugin::IPluginConfigSelector {
 public:
  using Selector = std::function<std::string()>;

  ///
  /// \brief Create a new selector
  /// \param key The configuration key to select a value for
  /// \param selector The selection helper function (returns the value)
  /// \param section The configuration key section name
  ///
  PluginConfigSelector(std::string key, const Selector& selector,
                       std::string section = "");

  ///
  /// \brief Create a new selector
  /// \param key The configuration key to select a value for
  /// \param applicable_values A list of applicable values for this
  /// configuration
  /// \param section The configuration key section name
  /// \param help_text The help text to display
  /// \details The constructed selector will check the input value to be present
  /// in the list of applicable values as well as actually display the list
  ///
  PluginConfigSelector(std::string key,
                       std::vector<std::string> applicable_values,
                       std::string section = "", std::string help_text = "");

  ///
  /// \brief Create a new selector
  /// \param key The configuration key
  /// \param value The configuration value to set
  /// \param section The configuration key section name
  /// \details This will directly set a value, use it to provide defaults
  ///
  PluginConfigSelector(std::string key, const std::string& value,
                       std::string section = "");

  ///
  /// \copydoc imonitorplugin::IPluginConfigSelector::SelectConfig()
  ///
  [[nodiscard]] std::tuple<std::string, std::string, std::string> SelectConfig()
      const override;

 private:
  std::string key_;
  Selector selector_;
  std::string section_;

  std::function<std::string()> BuildSelector(
      const std::string& help_text, std::vector<std::string> applicable_values);
};

}  // namespace monitoringpluginbase

#endif  // PLUGINCONFIGSELECTOR_HPP
