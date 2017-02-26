//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_OUTPUTSTREAM_HPP
#define SOCKET_WEBSERVER_OUTPUTSTREAM_HPP

#include <stdio.h>
#include <string>

class OutputStream {

public:

    virtual void write(void *buf, uint64_t length) = 0;
    virtual void writeRawString(std::string const &str) = 0;

};


#endif //SOCKET_WEBSERVER_OUTPUTSTREAM_HPP
