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
  ///
  PluginConfigSelector(std::string key, const Selector& selector);

  ///
  /// \brief Create a new selector
  /// \param key The configuration key to select a value for
  /// \param applicable_values A list of applicable values for this
  /// configuration
  /// \param help_text The help text to display
  /// \details The constructed selector will check the input value to be present
  /// in the list of applicable values as well as actually display the list
  ///
  PluginConfigSelector(std::string key,
                       std::vector<std::string> applicable_values,
                       std::string help_text = "");

  ///
  /// \copydoc imonitorplugin::IPluginConfigSelector::SelectConfig()
  ///
  [[nodiscard]] std::tuple<std::string, std::string> SelectConfig()
      const override;

 private:
  std::string key_;
  Selector selector_;

  std::function<std::string()> BuildSelector(
      const std::string& help_text, std::vector<std::string> applicable_values);
};

}  // namespace monitoringpluginbase

#endif  // PLUGINCONFIGSELECTOR_HPP
