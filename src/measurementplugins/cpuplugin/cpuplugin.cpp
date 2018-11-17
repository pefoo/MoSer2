#include "measurementplugins/cpuplugin/cpuplugin.hpp"

cpuplugin::CpuPlugin::CpuPlugin(std::string name)
    : measurementpluginbase::MeasurementPluginBase(std::move(name)) {}

measurementpluginbase::MeasurementPluginBase::data
cpuplugin::CpuPlugin::AcquireDataInternal() const {
  return data({{"testey", "test value"}});
}
