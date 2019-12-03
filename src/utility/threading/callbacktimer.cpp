#include "utility/threading/callbacktimer.hpp"
#include <chrono>
#include <exception>
#include <mutex>
#include "easyloggingpp-9.96.5/easylogging++.h"

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
  this->is_running_.store(true, std::memory_order_release);

  this->thread_ = std::thread([=]() {
    std::mutex mutex;
    std::unique_lock<std::mutex> lk(mutex);
    auto last_run = std::chrono::system_clock::now();
    while (this->is_running_.load(std::memory_order_acquire)) {
      if (this->cv_.wait_for(lk, std::chrono::milliseconds(interval_ms)) !=
          std::cv_status::timeout) {
        continue;
      }
      // Since condition variables tend to signal spurious, double check using a
      // calculated duration
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now() - last_run)
                         .count();
      if (elapsed < interval_ms) continue;

      last_run = std::chrono::system_clock::now();
      try {
        func();
      } catch (const std::exception& e) {
        LOG(ERROR) << "An error occurred during function execution in a "
                      "callback timer. "
                   << e.what();
      }
    }
  });
}

void CallbackTimer::Stop() {
  this->is_running_.store(false, std::memory_order_release);
  this->cv_.notify_one();
  if (this->thread_.joinable()) {
    this->thread_.join();
  }
}

bool CallbackTimer::is_running() const {
  return this->is_running_ && this->thread_.joinable();
}

}  // namespace threading
}  // namespace utility
