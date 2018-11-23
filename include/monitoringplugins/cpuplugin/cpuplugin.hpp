#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H

#include <string>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace cpuplugin {
///
/// \brief TODO
///
class CpuPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  explicit CpuPlugin(std::string name_);

 protected:
  data AcquireDataInternal() const override;

 private:
};
extern "C" imonitorplugin::IMonitorPlugin* create(const std::string& name) {
  return new CpuPlugin{name};
}

extern "C" void destroy(CpuPlugin* p) { delete p; }
}  // namespace cpuplugin
}  // namespace monitoringplugins
#endif  // CPUPLUGIN_H
