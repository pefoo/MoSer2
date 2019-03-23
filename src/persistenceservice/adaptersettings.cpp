#include "persistenceservice/adaptersettings.hpp"

persistenceservice::AdapterSettings::AdapterSettings(
    persistenceservice::AdapterTypes adapter_type)
    : adapter_type_(adapter_type) {}

persistenceservice::AdapterSettings::~AdapterSettings() = default;

persistenceservice::AdapterTypes
persistenceservice::AdapterSettings::adapter_type() const {
  return this->adapter_type_;
}
