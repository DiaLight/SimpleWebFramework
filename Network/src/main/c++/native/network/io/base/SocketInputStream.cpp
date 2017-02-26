//
// Created by dialight on 23.02.17.
//

#include "network/io/base/SocketInputStream.hpp"

#include <network/exceptions/IOException.hpp>
#include <sys/socket.h>

SocketInputStream::SocketInputStream(int socket) : socket(socket) {}

uint64_t SocketInputStream::read(void *buf, uint64_t offset, uint64_t length) {
    ssize_t len = ::recv(socket, ((uint8_t *) buf) + offset, length, MSG_DONTWAIT);
    if(len == 0) {
        throw IOException("SocketInputStream.read", "connection closed by client");
    }
    if(len < 0) {
        if(errno == EWOULDBLOCK) return 0;
        throw IOException("SocketInputStream.read");
    }
    return (uint64_t) len;
}

std::string SocketInputStream::readRawString() {
    throw IOException("SocketInputStream.readRawString", "Reading string in unlimited stream is unsupported, cause of unknown bounds of stream.");
}
