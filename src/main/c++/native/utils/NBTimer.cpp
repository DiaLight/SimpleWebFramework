//
// Created by dialight on 24.02.17.
//

#include "utils/NBTimer.hpp"
#include <chrono>

using namespace std::chrono;

NBTimer::NBTimer(long ns) : ns(ns), prev(system_clock::now().time_since_epoch().count()) {}

bool NBTimer::point() {
    long now = system_clock::now().time_since_epoch().count();
    if((now - prev) > ns) {
        prev = now;
        return true;
    }
    return false;
}
