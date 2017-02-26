//
// Created by dialight on 24.02.17.
//

#ifndef SOCKET_WEBSERVER_DATAOUTPUTSTREAM_HPP
#define SOCKET_WEBSERVER_DATAOUTPUTSTREAM_HPP


#include <network/io/base/OutputStream.hpp>

class DataOutputStream : public OutputStream {

    OutputStream *os;
public:
    DataOutputStream(OutputStream *os);

    void write(void *buf, uint64_t length) override;

    void writeRawString(std::string const &str) override;

    void writeByte(int8_t b);

    void writeShort(int16_t s);

    void writeInteger(int32_t i);

    void writeLong(int64_t l);

    void writeString(std::string const &str);

};


#endif //SOCKET_WEBSERVER_DATAOUTPUTSTREAM_HPP
