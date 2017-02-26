//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_WS_HPP
#define SOCKET_WEBSERVER_WS_HPP

#include <cstdint>
#include <network/io/base/OutputStream.hpp>

namespace ws {

    // Flags
    static uint8_t FIN =  0b10000000;
    static uint8_t RSV1 = 0b01000000;
    static uint8_t RSV2 = 0b00100000;
    static uint8_t RSV3 = 0b00010000;
    static uint8_t FLAGS_MASK = 0b11110000;

    // Opcodes
    static uint8_t TXT =  0x1;
    static uint8_t BIN =  0x2;
//    static uint8_t RES1 =  0x3;
//    static uint8_t RES2 =  0x4;
//    static uint8_t RES3 =  0x5;
//    static uint8_t RES4 =  0x6;
//    static uint8_t RES5 =  0x7;
    static uint8_t CLOSE = 0x8;
    static uint8_t PING =  0x9;
    static uint8_t PONG =  0xA;
//    static uint8_t RES6 =  0xB;
//    static uint8_t RES7 =  0xC;
//    static uint8_t RES8 =  0xD;
//    static uint8_t RES9 =  0xE;
//    static uint8_t RES10 = 0xF;
    static uint8_t OPCODE_MASK = 0b00001111;

    static uint8_t DEFAULT = FIN | BIN;

    // second byte
    static uint8_t MASK = 0b10000000;
    static uint8_t LEN =  0b01111111;

    // close codes
    static uint16_t NORMAL = 1000;
    static uint16_t PAGE_CLOSED = 1001;
    static uint16_t EXCEPTION = 1002;
    static uint16_t BAD_DATA = 1003;
    
    class Packet {
        uint8_t *data;
        uint64_t size;
        uint8_t *body;
    public:
        Packet();

        virtual ~Packet();

        void build(uint64_t len);

        void write(void *buf, uint64_t offset, uint64_t len);

        void send(OutputStream *os);

        void setFlags(uint8_t flags);

        void setOpcode(uint8_t opcode);
    };
    
    
}


#endif //SOCKET_WEBSERVER_WS_HPP
