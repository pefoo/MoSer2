#include "utility/threading/callbacktimer.hpp"
#include <chrono>
#include <exception>
#include <mutex>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

namespace utility {
namespace threading {

CallbackTimer::CallbackTimer() : is_running_(false) {}

CallbackTimer::~CallbackTimer() {
  if (this->is_running()) {
    this->Stop();
  }
}

void CallbackTimer::Start(int interval_ms, const std::function<void()>& func) {
  if (this->is_running()) {
    this->Stop();
  }

  this->thread_ = std::thread([=]() {
    std::mutex mutex;
    std::unique_lock<std::mutex> lk(mutex);
    while (true) {
      if (this->cv_.wait_for(lk, std::chrono::milliseconds(interval_ms)) ==
          std::cv_status::no_timeout) {
        break;
      }
      try {
        func();
      } catch (const std::exception& e) {
        LOG(ERROR) << "An error occurred during function execution in a "
                      "callback timer. "
                   << e.what();
      }
    }
  });
  this->is_running_ = true;
}

void CallbackTimer::Stop() {
  this->cv_.notify_one();
  if (this->thread_.joinable()) {
    this->thread_.join();
  }
  this->is_running_ = false;
}

bool CallbackTimer::is_running() const {
  return this->is_running_ && this->thread_.joinable();
}

}  // namespace threading
}  // namespace utility
