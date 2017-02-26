//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_BYTEARRAYINPUTSTREAM_HPP
#define SOCKET_WEBSERVER_BYTEARRAYINPUTSTREAM_HPP


#include "InputStream.hpp"

class ByteArrayInputStream : public InputStream {
    uint8_t *buf;
    uint8_t *end;
    uint8_t *pos;
public:
    ByteArrayInputStream(void *buf, uint64_t bufLen);

    uint64_t read(void *buf, uint64_t offset, uint64_t length) override;

    std::string readRawString() override;
};


#endif //SOCKET_WEBSERVER_BYTEARRAYINPUTSTREAM_HPP
