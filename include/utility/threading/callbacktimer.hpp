#ifndef CALLBACKTIMER_H
#define CALLBACKTIMER_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <thread>

namespace utility {
namespace threading {
///
/// \brief A simple callback timer
///
class CallbackTimer {
 public:
  ///
  /// \brief CallbackTimer
  ///
  CallbackTimer();

  ///
  /// \brief ~CallbackTimer
  ///
  ~CallbackTimer();

  ///
  /// \brief Start the timer
  /// \param interval_ms The interval in ms
  /// \param func The function to call
  ///
  void Start(int interval_ms, const std::function<void(void)>& func);

  ///
  /// \brief Stop the timer
  ///
  void Stop();

  ///
  /// \brief Get whether the timer is runner
  /// \return True, if the timer is running
  ///
  [[nodiscard]] bool is_running() const;

 private:
  std::atomic_bool is_running_;
  std::thread thread_;
  std::condition_variable cv_;
};

}  // namespace threading
}  // namespace utility

#endif  // CALLBACKTIMER_H
