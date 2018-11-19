#include "monitoringplugins/cpuplugin/cpuplugin.hpp"

monitoringplugins::cpuplugin::CpuPlugin::CpuPlugin(std::string name)
    : monitoringpluginbase::MonitorPluginBase(std::move(name)) {}

monitoringpluginbase::MonitorPluginBase::data
monitoringplugins::cpuplugin::CpuPlugin::AcquireDataInternal() const {
  return data({{"testey", "test value"}});
}
