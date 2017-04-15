//
// Created by dialight on 24.02.17.
//

#ifndef SOCKET_WEBSERVER_TIMER_HPP
#define SOCKET_WEBSERVER_TIMER_HPP

#include <ctime>

class SyncTimer {
    std::clock_t delay;
    std::clock_t prev;
    std::function<void ()> runnable;
public:
    SyncTimer(std::clock_t delay, std::function<void ()> const &run);

    void tick();

};


#endif //SOCKET_WEBSERVER_TIMER_HPP
