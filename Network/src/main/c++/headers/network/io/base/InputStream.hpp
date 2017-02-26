//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_INPUTSTREAM_HPP
#define SOCKET_WEBSERVER_INPUTSTREAM_HPP

#include <stdio.h>
#include <string>

class InputStream {

public:

    virtual uint64_t read(void *buf, uint64_t offset, uint64_t length) = 0;

    virtual std::string readRawString() = 0;

};


#endif //SOCKET_WEBSERVER_INPUTSTREAM_HPP
