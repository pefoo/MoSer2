#include "utility/threading/callbacktimer.hpp"
#include <exception>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

namespace utility {
namespace threading {

CallbackTimer::CallbackTimer() : execute_(false) {}

CallbackTimer::~CallbackTimer() {
  if (this->execute_.load(std::memory_order_acquire)) {
    this->Stop();
  }
}

void CallbackTimer::Start(int interval_ms, const std::function<void()>& func) {
  if (this->is_running()) {
    this->Stop();
  }

  this->execute_.store(true, std::memory_order_release);
  this->thread_ = std::thread([=]() {
    while (this->execute_.load(std::memory_order_acquire)) {
      try {
        func();
      } catch (const std::exception& e) {
        LOG(ERROR) << "An error occurred during function execution in a "
                      "callback timer. "
                   << e.what();
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
  });
}

void CallbackTimer::Stop() {
  this->execute_.store(false, std::memory_order_release);
  if (this->thread_.joinable()) {
    this->thread_.join();
  }
}

bool CallbackTimer::is_running() const {
  return this->execute_.load(std::memory_order_acquire) &&
         this->thread_.joinable();
}

}  // namespace threading
}  // namespace utility
