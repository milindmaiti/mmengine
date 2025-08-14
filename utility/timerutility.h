#pragma once
#include "macros.h"
#include <chrono>
#include <memory>
ull getMilliseconds();

void threadTimer(std::chrono::milliseconds msec,
                 std::atomic<bool>* timerFlag);
