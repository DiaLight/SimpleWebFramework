//
// Created by dialight on 24.02.17.
//

#include <utils/SyncTimer.hpp>

using namespace std;

SyncTimer::SyncTimer(clock_t delay, function<void ()> const &run) :
        delay(delay),
        prev(clock()),
        runnable(run) {}

static const clock_t CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;

void SyncTimer::tick() {
    clock_t now = clock();
    clock_t duration = ( now - prev ) / CLOCKS_PER_MS;
    if(duration > delay) {
        prev = now;
        runnable();
    }
}
