//
// Created by dialight on 24.02.17.
//

#ifndef SOCKET_WEBSERVER_DATAINPUTSTREAM_HPP
#define SOCKET_WEBSERVER_DATAINPUTSTREAM_HPP


#include <network/io/base/InputStream.hpp>

class DataInputStream : public InputStream {

    InputStream *is;
public:
    DataInputStream(InputStream *is);

    uint64_t read(void *buf, uint64_t offset, uint64_t length) override;

    std::string readRawString() override;

    uint8_t readByte();

    uint16_t readShort();

    uint32_t readInteger();

    uint64_t readLong();

    std::string readString();
};


#endif //SOCKET_WEBSERVER_DATAINPUTSTREAM_HPP
