//
// Created by dialight on 23.02.17.
//

#include <network/exceptions/IOException.hpp>
#include <cstring>
#include "network/io/base/ByteArrayInputStream.hpp"

ByteArrayInputStream::ByteArrayInputStream(void *buf, uint64_t bufLen) : buf((uint8_t *) buf), end((uint8_t *) buf + bufLen), pos((uint8_t *)buf) {}

uint64_t ByteArrayInputStream::read(void *buf, uint64_t offset, uint64_t length) {
    if(pos + offset + length > end) {
        length = end - pos - offset;
        if(length < 0) throw IOException("ByteArrayInputStream.read", "offset out of bounds");
    }
    if(length == 0) return 0;
    memcpy(buf, pos, length);
    pos += length;
    return length;
}

std::string ByteArrayInputStream::readRawString() {
    return std::string((char *) pos, end - pos);
}
