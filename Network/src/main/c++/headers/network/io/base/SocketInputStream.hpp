//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_SOCKETINPUTSTREAM_HPP
#define SOCKET_WEBSERVER_SOCKETINPUTSTREAM_HPP


#include "InputStream.hpp"

class SocketInputStream : public InputStream {

    int socket;
public:
    SocketInputStream(int socket);

    uint64_t read(void *buf, uint64_t offset, uint64_t length) override;
    std::string readRawString() override;

};


#endif //SOCKET_WEBSERVER_SOCKETINPUTSTREAM_HPP
