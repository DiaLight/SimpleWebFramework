//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_SOCKETOUTPUTSTREAM_HPP
#define SOCKET_WEBSERVER_SOCKETOUTPUTSTREAM_HPP


#include "OutputStream.hpp"

class SocketOutputStream : public OutputStream {

    int socket;
public:
    SocketOutputStream(int socket);

    void write(void *buf, uint64_t length) override;

    void writeRawString(std::string const &str) override;

};


#endif //SOCKET_WEBSERVER_SOCKETOUTPUTSTREAM_HPP
