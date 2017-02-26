//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_BYTEARRAYOUTPUTSTREAM_HPP
#define SOCKET_WEBSERVER_BYTEARRAYOUTPUTSTREAM_HPP


#include "OutputStream.hpp"

class ByteArrayOutputStream : public OutputStream {
    uint8_t *buf;
    uint8_t *pos;
    uint8_t *end;
public:
    ByteArrayOutputStream();

    virtual ~ByteArrayOutputStream();

    void write(void *buf, uint64_t length) override;

    void writeRawString(std::string const &str) override;

    uint8_t *getBuffer();

    uint64_t size();

    ByteArrayOutputStream(void *buf, uint64_t length);

};


#endif //SOCKET_WEBSERVER_BYTEARRAYOUTPUTSTREAM_HPP
