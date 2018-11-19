#ifndef IMONITORPLUGIN_H
#define IMONITORPLUGIN_H

#include <string>
namespace imonitorplugin {
class PluginData;
///
/// \brief The interface monitoring plugins have to implement
///
class IMonitorPlugin {
 public:
  virtual ~IMonitorPlugin() {}
  virtual std::string GetName() const = 0;
  virtual PluginData AcquireData() const = 0;
};

typedef IMonitorPlugin* create_t(const std::string& name);
typedef void destroy_t(IMonitorPlugin*);
}  // namespace imonitorplugin
#endif  // IMONITORPLUGIN_H
