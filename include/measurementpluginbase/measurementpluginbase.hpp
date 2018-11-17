#ifndef MEASUREMENTPLUGINBASE_H
#define MEASUREMENTPLUGINBASE_H

#include <string>
#include <vector>
#include "monitorplugin/imonitorplugin.hpp"

namespace measurementpluginbase {
///
/// \brief The base class of each monitoring plugin.
/// \details Provides basic functionality for plugins
///
class MeasurementPluginBase : virtual public monitorplugin::IMonitorPlugin {
 public:
  ///
  /// \brief Create new instance
  /// \param name The name of the plugin
  ///
  MeasurementPluginBase(std::string name);

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  std::string GetName() const override;

  ///
  /// \brief Ask the plugin to acquire the data it was written for
  /// \return A plugin data objection that contains the information the plugin
  /// acquired
  ///
  monitorplugin::PluginData AcquireData() const override;

 protected:
  typedef std::vector<std::pair<std::string, std::string>> data;
  ///
  /// \brief Here the actual work happens. Plugins override this method.
  /// \details Plugins are not responsible for packing the data.
  /// \return A vector of key value pairs
  ///
  virtual data AcquireDataInternal() const = 0;

  ///
  /// \brief Sleep 100ms. Plugins that need to calculate their stats using
  /// consecutive reads should employ this method
  ///
  void Sleep100ms() const;

 private:
  std::int64_t MakeTimestamp() const;
  std::string name;
};
}
#endif  // MEASUREMENTPLUGINBASE_H
