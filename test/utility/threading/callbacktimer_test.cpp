#include "utility/threading/callbacktimer.hpp"
#include <condition_variable>
#include <mutex>
#include "catch2/catch.hpp"

///
/// \brief Callback timer basic functionality
///
TEST_CASE("Callback timer basic functionality", "[utility]") {
  utility::threading::CallbackTimer cb{};
  std::condition_variable cv;
  std::mutex mut;

  cb.Start(100, [&cv]() { cv.notify_one(); });
  std::unique_lock<std::mutex> lk(mut);

  // timer is running, wait_for should not time out since the timer
  // calls notify_one()
  auto status = cv.wait_for(lk, std::chrono::milliseconds(250));
  REQUIRE(status == std::cv_status::no_timeout);

  cb.Stop();

  // timer is not running, wait_for should time out.
  status = cv.wait_for(lk, std::chrono::milliseconds(250));
  REQUIRE(status == std::cv_status::timeout);
}

///
/// \brief Callback timer auto stop functionality
///
TEST_CASE("Callback timer auto stop", "[utility]") {
  utility::threading::CallbackTimer* cb =
      new utility::threading::CallbackTimer{};
  std::condition_variable cv;
  std::mutex mut;

  // Timer stops when the destructor is called
  cb->Start(100, [&cv]() { cv.notify_one(); });
  delete cb;
  std::unique_lock<std::mutex> lk(mut);
  auto status = cv.wait_for(lk, std::chrono::milliseconds(250));
  REQUIRE(status == std::cv_status::timeout);

  // Timer stops when start is called again
  cb = new utility::threading::CallbackTimer{};
  cb->Start(50, [&cv]() { cv.notify_one(); });
  cb->Start(200, [&cv]() { cv.notify_one(); });
  status = cv.wait_for(lk, std::chrono::milliseconds(100));
  REQUIRE(status == std::cv_status::timeout);
}
