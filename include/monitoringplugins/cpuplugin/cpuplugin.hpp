#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H

#include "monitoringpluginbase/measurementpluginbase.hpp"
namespace monitoringplugins {
namespace cpuplugin {
///
/// \brief TODO
///
class CpuPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  CpuPlugin(std::string name);

 protected:
  data AcquireDataInternal() const override;

 private:
};
extern "C" imonitorplugin::IMonitorPlugin* create(const std::string& name) {
  return new CpuPlugin{name};
}

extern "C" void destroy(CpuPlugin* p) { delete p; }
}
}
#endif  // CPUPLUGIN_H
