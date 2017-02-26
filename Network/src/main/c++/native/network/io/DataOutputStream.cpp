//
// Created by dialight on 24.02.17.
//

#include <netinet/in.h>
#include "network/io/DataOutputStream.hpp"

DataOutputStream::DataOutputStream(OutputStream *os) : os(os) {}


void DataOutputStream::write(void *buf, uint64_t length) {
    os->write(buf, length);
}

void DataOutputStream::writeRawString(std::string const &str) {
    os->writeRawString(str);
}

void DataOutputStream::writeByte(int8_t b) {
    os->write(&b, 1);
}

void DataOutputStream::writeShort(int16_t s) {
    uint16_t ns = htons(s);
    os->write(&ns, 2);
}

void DataOutputStream::writeInteger(int32_t i) {
    uint32_t ni = htonl(i);
    os->write(&ni, 4);
}

void DataOutputStream::writeLong(int64_t l) {
    uint64_t nl = __bswap_64(l);
    os->write(&nl, 8);
}

void DataOutputStream::writeString(std::string const &str) {
    writeShort((uint16_t) str.size());
    os->write((void *) str.c_str(), str.size());
}
