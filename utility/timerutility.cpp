#include "../utility/macros.h"
#include <chrono>
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
