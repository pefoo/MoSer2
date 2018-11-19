#include "monitoringplugins/cpuplugin/cpuplugin.hpp"
#include <string>
#include <utility>

monitoringplugins::cpuplugin::CpuPlugin::CpuPlugin(std::string name)
    : monitoringpluginbase::MonitorPluginBase(std::move(name)) {}

monitoringpluginbase::MonitorPluginBase::data
monitoringplugins::cpuplugin::CpuPlugin::AcquireDataInternal() const {
  return data({{"testey", "test value"}});
}
