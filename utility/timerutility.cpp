#include "../utility/macros.h"
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
ull getMilliseconds() {
  // Get the current time from the system clock
  auto now = std::chrono::system_clock::now();

  // Convert the current time to time since epoch
  auto duration = now.time_since_epoch();

  // Convert duration to milliseconds
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  return static_cast<ull>(milliseconds);
}

// sets a thread to sleep for time milliseconds
void threadTimer(std::chrono::milliseconds msec,
                 std::atomic<bool>* timerFlag) {
  std::this_thread::sleep_for(msec);
  *timerFlag = true;
}
