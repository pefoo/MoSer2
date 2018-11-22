#include "utility/threading/callbacktimer.hpp"
#include <exception>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

namespace utility {
namespace threading {

CallbackTimer::CallbackTimer() : execute(false) {}

CallbackTimer::~CallbackTimer() {
  if (this->execute.load(std::memory_order_acquire)) {
    this->Stop();
  }
}

void CallbackTimer::Start(int interval_ms, const std::function<void()>& func) {
  if (this->is_running()) {
    this->Stop();
  }

  this->execute.store(true, std::memory_order_release);
  this->thread = std::thread([=]() {
    while (this->execute.load(std::memory_order_acquire)) {
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
  this->execute.store(false, std::memory_order_release);
  if (this->thread.joinable()) {
    this->thread.join();
  }
}

bool CallbackTimer::is_running() const {
  return this->execute.load(std::memory_order_acquire) &&
         this->thread.joinable();
}

}  // namespace threading
}  // namespace utility
