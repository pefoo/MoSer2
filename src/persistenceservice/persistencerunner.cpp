#include "persistenceservice/persistencerunner.hpp"
#include "utility"
#include "utility/threading/callbacktimer.hpp"

persistenceservice::PersistenceRunner::PersistenceRunner(
    std::unique_ptr<persistenceservice::IDataAdapter> data_adapter,
    std::function<bool(imonitorplugin::PluginData *)> source)
    : data_adapter_(std::move(data_adapter)), source_func_(std::move(source)) {}

void persistenceservice::PersistenceRunner::Run(int delay) {
  this->execute_.store(true, std::memory_order_release);
  this->thread_ = std::thread([&]() {
    utility::threading::CallbackTimer cb_timer{};
    std::mutex mutex;
    cb_timer.Start(delay, [&]() { this->cv_.notify_one(); });

    while (this->execute_.load(std::memory_order_acquire)) {
      std::unique_lock<std::mutex> lk(mutex);
      this->cv_.wait(lk);
      if (!this->execute_.load(std::memory_order_acquire)) break;

      imonitorplugin::PluginData d;
      while (this->source_func_(&d)) {
        this->data_adapter_->Store(d);
      }
    }
    cb_timer.Stop();
  });
}

void persistenceservice::PersistenceRunner::Stop() {
  this->execute_.store(false, std::memory_order_release);
  this->cv_.notify_one();
  if (this->thread_.joinable()) {
    this->thread_.join();
  }
}
