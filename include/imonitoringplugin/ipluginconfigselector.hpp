#ifndef IPLUGINCONFIGSELECTOR_HPP
#define IPLUGINCONFIGSELECTOR_HPP

#include <functional>
#include <string>
#include <tuple>

namespace imonitorplugin {

///
/// \brief The IPluginConfigSelector class
///
class IPluginConfigSelector {
 public:
  ///
  /// ~IPluginConfigSelector
  ///
  virtual ~IPluginConfigSelector() = default;

  ///
  /// \brief Select the config value
  /// \return The config key, the section name and the selected value
  ///
  [[nodiscard]] virtual std::tuple<std::string, std::string, std::string>
  SelectConfig() const = 0;
};

}  // namespace imonitorplugin
#endif  // IPLUGINCONFIGSELECTOR_HPP
