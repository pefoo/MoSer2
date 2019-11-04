#include "persistenceservice/persistencerunner.hpp"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include "utility/threading/callbacktimer.hpp"

persistenceservice::PersistenceRunner::PersistenceRunner(
    std::unique_ptr<persistenceservice::IDataAdapter> data_adapter,
    std::function<bool(imonitorplugin::PluginData*)> source)
    : data_adapter_(std::move(data_adapter)),
      source_func_(std::move(source)),
      timer_(new utility::threading::CallbackTimer{}) {}

void persistenceservice::PersistenceRunner::Run(int delay) {
  this->timer_->Start(delay, [&]() {
    std::vector<imonitorplugin::PluginData> data;
    imonitorplugin::PluginData d;
    while (this->source_func_(&d)) {
      data.push_back(imonitorplugin::PluginData{d});
    }
    if (!data.empty()) this->data_adapter_->Store(data.begin(), data.end());
  });
}

void persistenceservice::PersistenceRunner::Stop() { this->timer_->Stop(); }
