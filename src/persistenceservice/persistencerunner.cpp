#include "persistenceservice/persistencerunner.hpp"
#include <memory>
#include <utility>
#include "utility/threading/callbacktimer.hpp"

persistenceservice::PersistenceRunner::PersistenceRunner(
    std::unique_ptr<persistenceservice::IDataAdapter> data_adapter,
    std::function<bool(imonitorplugin::PluginData *)> source)
    : data_adapter_(std::move(data_adapter)),
      source_func_(std::move(source)),
      timer_(new utility::threading::CallbackTimer{}) {}

void persistenceservice::PersistenceRunner::Run(int delay) {
  this->timer_->Start(delay, [&]() {
    imonitorplugin::PluginData d;
    while (this->source_func_(&d)) {
      this->data_adapter_->Store(d);
    }
  });
}

void persistenceservice::PersistenceRunner::Stop() { this->timer_->Stop(); }
