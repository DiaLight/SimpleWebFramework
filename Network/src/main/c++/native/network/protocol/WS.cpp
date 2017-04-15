//
// Created by dialight on 23.02.17.
//

#include "network/protocol/WS.hpp"

#include <cstring>

using namespace std;

namespace ws {
    Packet::Packet() : data(nullptr), size(0) {}

    Packet::~Packet() {
        if(data != nullptr) delete[] data;
    }

    void Packet::build(uint64_t len, uint8_t flags, uint8_t opcodes) {
        if (len < 126) {
            size = 2 + len;
            data = new uint8_t[size];
            body = data + 2;
            data[1] = (uint8_t) len;
        } else if (len < 65533) {
            size = 4 + len;
            data = new uint8_t[size];
            body = data + 4;
            data[1] = 126;
            data[2] = (uint8_t) (len >> 8);
            data[3] = (uint8_t) len;
        } else {
            size = 10 + len;
            data = new uint8_t[size];
            body = data + 10;
            data[1] = 127;
            data[2] = (uint8_t) (len >> 56);
            data[3] = (uint8_t) (len >> 48);
            data[4] = (uint8_t) (len >> 40);
            data[5] = (uint8_t) (len >> 32);
            data[6] = (uint8_t) (len >> 24);
            data[7] = (uint8_t) (len >> 16);
            data[8] = (uint8_t) (len >> 8);
            data[9] = (uint8_t) len;
        }
        data[0] = flags | opcodes;
    }

    void Packet::write(void *buf, uint64_t offset, uint64_t len) {
        memcpy(body + offset, buf, len);
    }
    void Packet::send(OutputStream *os) {
//        for (int i = 0; i < size; ++i) cout << hex << setfill('0') << setw(2) << (int) data[i] << " "; cout << endl;
        os->write(data, size);
    }

    void Packet::setFlags(uint8_t flags) {
        data[0] = data[0] & opcode::MASK | flags & flag::MASK;
    }

    void Packet::setOpcode(uint8_t opcode) {
        data[0] = data[0] & flag::MASK | opcode & opcode::MASK;
    }
}
