#ifndef IMONITORPLUGIN_H
#define IMONITORPLUGIN_H

#include <string>
#include "imonitoringplugin/plugindata.hpp"
namespace imonitorplugin {
///
/// \brief The interface monitoring plugins have to implement
///
class IMonitorPlugin {
 public:
  virtual ~IMonitorPlugin() {}
  virtual std::string name() const = 0;
  virtual PluginData AcquireData() const = 0;
};

}  // namespace imonitorplugin
#endif  // IMONITORPLUGIN_H
