//
// Created by dialight on 23.02.17.
//

#include "network/io/base/SocketOutputStream.hpp"

#include <network/exceptions/IOException.hpp>
#include <sys/socket.h>
#include <iostream>

SocketOutputStream::SocketOutputStream(int socket) : socket(socket) {}

void SocketOutputStream::write(void *buf, uint64_t length) {
    ssize_t sent;
    while((sent = ::send(socket, buf, length, 0)) < length) {
        if(sent <= 0) {
            throw IOException("SocketOutputStream.write");
        }
        buf = ((uint8_t *) buf) + sent;
        length -= sent;
    }
}

void SocketOutputStream::writeRawString(std::string const &str) {
    write((uint8_t *) str.c_str(), str.length());
}
