#ifndef MEASUREMENTPLUGINBASE_H
#define MEASUREMENTPLUGINBASE_H

#include <string>
#include <utility>
#include <vector>
#include "imonitoringplugin/imonitorplugin.hpp"

namespace monitoringpluginbase {
/*
 * Macros to define the default function factories.
 * Pass the plugin type as argument.
 * Note that these macros may not be placed inside a class!
 */
#define CREATE_DETAULT_CONSTRUCTOR_FACTORY(type) \
  extern "C" ::imonitorplugin::IMonitorPlugin* create() { return new type{}; }

#define CREATE_DEFAULT_DESTRUCTOR_FACTORY(type) \
  extern "C" void destroy(type* p) { delete p; }

///
/// \brief The base class of each monitoring plugin.
/// \details Provides basic functionality for plugins
///
class MonitorPluginBase : virtual public imonitorplugin::IMonitorPlugin {
 public:
  ///
  /// \brief Create new instance
  /// \param name The name of the plugin
  ///
  explicit MonitorPluginBase(std::string name_);

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  std::string name() const override;

  ///
  /// \brief Ask the plugin to acquire the data it was written for
  /// \return A plugin data objection that contains the information the plugin
  /// acquired
  ///
  imonitorplugin::PluginData AcquireData() const override;

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
  std::string name_;
};
}  // namespace monitoringpluginbase
#endif  // MEASUREMENTPLUGINBASE_H
