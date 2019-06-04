#include "persistenceservice/adapterfactory.hpp"
#include <memory>
#include <utility>
#include "persistenceservice/AdapterTypes.hpp"
#include "persistenceservice/sqlite/sqliteadapter.hpp"

persistenceservice::AdapterFactory::AdapterFactory(
    std::unique_ptr<AdapterSettings> adapter_settings)
    : adapter_settings_(std::move(adapter_settings)) {}

std::unique_ptr<persistenceservice::IDataAdapter>
persistenceservice::AdapterFactory::CreateAdapter() const {
  switch (this->adapter_settings_->adapter_type()) {
    case persistenceservice::AdapterTypes::sqlite:
      return std::make_unique<sqlite::SqliteAdapter>(
          this->adapter_settings_.get());
  }
  throw std::runtime_error("Adapter type not supported");
}
