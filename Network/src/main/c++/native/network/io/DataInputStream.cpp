//
// Created by dialight on 24.02.17.
//

#include "network/io/DataInputStream.hpp"

#include <network/exceptions/IOException.hpp>
#include <netinet/in.h>

DataInputStream::DataInputStream(InputStream *is) : is(is) {}

uint64_t DataInputStream::read(void *buf, uint64_t offset, uint64_t length) {
    return is->read(buf, offset, length);
}

std::string DataInputStream::readRawString() {
    return is->readRawString();
}

uint8_t DataInputStream::readByte() {
    uint8_t b = 0;
    if(is->read(&b, 0, 1) != 1) {
        throw IOException("DataInputStream.readByte");
    }
    return b;
}

uint16_t DataInputStream::readShort() {
    uint16_t s = 0;
    if(is->read(&s, 0, 2) != 2) {
        throw IOException("DataInputStream.readShort");
    }
    return ntohs(s);
}

uint32_t DataInputStream::readInteger() {
    uint32_t i = 0;
    if(is->read(&i, 0, 4) != 4) {
        throw IOException("DataInputStream.readInteger");
    }
    return ntohl(i);
}

uint64_t DataInputStream::readLong() {
    uint64_t l = 0;
    if(is->read(&l, 0, 4) != 4) {
        throw IOException("DataInputStream.readInteger");
    }
    return __bswap_64(l);
}

std::string DataInputStream::readString() {
    uint16_t size = readShort();
    char buf[size];
    if(is->read(buf, 0, size) != size) {
        throw IOException("DataInputStream.readString");
    }
    return std::string(buf, size);
}
