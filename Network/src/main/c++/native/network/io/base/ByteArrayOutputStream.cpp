//
// Created by dialight on 23.02.17.
//

#include <cstring>
#include "network/io/base/ByteArrayOutputStream.hpp"

ByteArrayOutputStream::ByteArrayOutputStream() {
    uint64_t capacity = 16;
    buf = new uint8_t[capacity];
    pos = buf;
    end = buf + capacity;
}

ByteArrayOutputStream::ByteArrayOutputStream(void *buf, uint64_t length) : buf((uint8_t *) buf) {
    pos = this->buf + length;
    end = pos;
}

ByteArrayOutputStream::~ByteArrayOutputStream() {
    delete[] buf;
}

void ByteArrayOutputStream::write(void *buf, uint64_t length) {
    if(pos + length >= end) {
        uint64_t capacity = end - this->buf;
        uint64_t size = pos - this->buf;
        uint64_t requiredSize = size + length;

        while (capacity < requiredSize) {
            capacity = (capacity * 3) / 2 + 1;
        }

        uint8_t *newBuf = new uint8_t[capacity];
        memcpy(newBuf, this->buf, size);

        delete this->buf;
        this->buf = newBuf;
        pos = newBuf + size;
        end = this->buf + capacity;
    }
    memcpy(pos, buf, length);
    pos += length;
}

void ByteArrayOutputStream::writeRawString(std::string const &str) {
    write((uint8_t *) str.c_str(), str.size());
}

uint8_t *ByteArrayOutputStream::getBuffer() {
    return buf;
}

uint64_t ByteArrayOutputStream::size() {
    return pos - buf;
}

