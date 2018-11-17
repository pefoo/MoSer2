#include "measurementpluginbase/measurementpluginbase.hpp"
#include <ctime>
#include "monitorplugin/plugindata.hpp"

measurementpluginbase::MeasurementPluginBase::MeasurementPluginBase(
    std::string name)
    : name(std::move(name)) {}

std::string measurementpluginbase::MeasurementPluginBase::GetName() const {
  return this->name;
}

monitorplugin::PluginData
measurementpluginbase::MeasurementPluginBase::AcquireData() const {
  return monitorplugin::PluginData{this->GetName(), this->MakeTimestamp(),
                                   this->AcquireDataInternal()};
}

void measurementpluginbase::MeasurementPluginBase::Sleep100ms() const {
  struct timespec dur = {0, 0};
  dur.tv_sec = 0;
  dur.tv_nsec = 100000000;
  nanosleep(&dur, (struct timespec*)nullptr);
}

std::int64_t measurementpluginbase::MeasurementPluginBase::MakeTimestamp()
    const {
  return static_cast<std::int64_t>(std::time(nullptr));
}
