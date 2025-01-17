#pragma once
#include "macros.h"
#include <memory>
ull getMilliseconds();

void threadTimer(std::chrono::milliseconds msec,
                 std::shared_ptr<std::atomic<bool>> timerFlag);
