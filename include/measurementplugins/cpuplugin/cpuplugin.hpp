#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H

#include "measurementpluginbase/measurementpluginbase.hpp"
namespace cpuplugin {
///
/// \brief TODO
///
class CpuPlugin : public measurementpluginbase::MeasurementPluginBase {
 public:
  CpuPlugin(std::string name);

 protected:
  data AcquireDataInternal() const override;

 private:
};
extern "C" monitorplugin::IMonitorPlugin* create(const std::string& name) {
  return new CpuPlugin{name};
}

extern "C" void destroy(CpuPlugin* p) { delete p; }
}

#endif  // CPUPLUGIN_H
