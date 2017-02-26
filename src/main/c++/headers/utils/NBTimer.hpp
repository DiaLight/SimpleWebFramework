//
// Created by dialight on 24.02.17.
//

#ifndef SOCKET_WEBSERVER_TIMER_HPP
#define SOCKET_WEBSERVER_TIMER_HPP


#include <chrono>

class NBTimer {
    long ns;
    long prev;
public:
    NBTimer(long ns);

    bool point();
};


#endif //SOCKET_WEBSERVER_TIMER_HPP
