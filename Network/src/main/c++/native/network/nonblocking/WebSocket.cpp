//
// Created by dialight on 22.02.17.
//

#include "network/nonblocking/WebSocket.hpp"

#include <network/io/base/ByteArrayInputStream.hpp>
#include <network/protocol/WS.hpp>

using namespace std;

void WebSocketHandler::onWsOpen(WebSocketListener listener) {
    onOpenListeners.push_back(listener);
}

void WebSocketHandler::onWsMessage(WebSocketMessageListener listener) {
    onMessageListeners.push_back(listener);
}

void WebSocketHandler::onWsClose(WebSocketCloseListener listener) {
    onCloseListeners.push_back(listener);
}

void WebSocketHandler::onWsError(WebSocketErrorListener listener) {
    onErrorListeners.push_back(listener);
}

void WebSocketHandler::fireOpen(WebSocket *webSocket) {
    for(auto const &listener : onOpenListeners) {
        listener(webSocket);
    }
}

void WebSocketHandler::fireMessage(WebSocket *webSocket, InputStream *message) {
    for(auto const &listener : onMessageListeners) {
        listener(webSocket, message);
    }
}

void WebSocketHandler::fireClose(WebSocket *webSocket, int code, string const &reason) {
    for(auto const &listener : onCloseListeners) {
        listener(webSocket, code, reason);
    }
}

void WebSocketHandler::fireError(WebSocket *webSocket, IOException &e) {
    for(auto const &listener : onErrorListeners) {
        listener(webSocket, e);
    }
}

WebSocket::WebSocket(TcpSocket *socket, WebSocketHandler *wsHandler) : socket(socket), wsHandler(wsHandler) {
    wsHandler->fireOpen(this);
}

WebSocket::~WebSocket() {

}

void WebSocket::tick() {
    uint64_t s;
    uint8_t tmp;

    InputStream *is = socket->getInputStream();
    
    s = is->read(&tmp, 0, 1);
    if (s == 0) return;
    int flags = tmp & ws::FLAGS_MASK;
    int opcode = tmp & ws::OPCODE_MASK;
    
    s = is->read(&tmp, 0, 1);
    if (s == 0) return;
    
    bool masked = tmp & ws::MASK;
    uint64_t len = tmp & ws::LEN;
    if (!masked) throw IOException("WebSocket.tick", "unmasked frame ignored");

    if (len == 126) {
        uint8_t tb[2];
        s = is->read(tb, 0, 2);
        if(s == 0) return;

        len = (uint64_t) (tb[0] * 256 + tb[1]);
    } else {
        if (len == 127) {
            uint8_t eb[8];
            uint64_t eightbyte = 0;
            s = is->read(eb, 0, 8);
            if(s == 0) return;

            for (int i = 0; i < 8; i++) {
                eightbyte = (eightbyte * 256) + eb[i];
            }
            len = eightbyte;
        }
    }

    uint8_t maskedData[512];
    uint8_t unMaskedData[len + 1];
    uint8_t maskingKey[4];
    s = is->read(maskingKey, 0, 4);
    if(s == 0) return;
    uint64_t received = 0;
    while (received < len) {
        s = is->read(maskedData, 0, 512);
        if(s == 0) continue;
        for (int i = 0; i < s; i++) {
            unMaskedData[received + i] = (uint8_t) ((int) maskedData[i] ^ (int) maskingKey[(received + i) % 4]);
        }
        received += s;
    }
    unMaskedData[len] = '\0';

    if(opcode == ws::CLOSE) {
        opened = false;
        if(len < 2) {
            wsHandler->fireClose(this, ws::NORMAL, "");
        } else {
            int code = unMaskedData[0] * 256 + unMaskedData[1];
            if(len == 2) {
                wsHandler->fireClose(this, code, "");
            } else {
                wsHandler->fireClose(this, code, string(((char *) unMaskedData) + 2, len - 2));
            }
        }
    } else {
        ByteArrayInputStream bais(unMaskedData, len);
        wsHandler->fireMessage(this, &bais);
    }
}

void WebSocket::sendMessage(ByteArrayOutputStream &baos) {
    ws::Packet p;
    p.build(baos.size());
    p.write(baos.getBuffer(), 0, baos.size());
    p.send(socket->getOutputStream());
}

TcpSocket *WebSocket::tcp() {
    return socket;
}

bool WebSocket::isOpen() {
    return opened;
}

void WebSocket::close(int code, string const &reason) {
    opened = false;
    uint8_t codeBuf[2];
    codeBuf[0] = (uint8_t) (code >> 8);
    codeBuf[1] = (uint8_t) code;

    ws::Packet p;
    p.build(2 + reason.size());
    p.setOpcode(ws::CLOSE);
    p.write(codeBuf, 0, 2);
    p.write((void *) reason.c_str(), 2, reason.size());
    p.send(socket->getOutputStream());
    wsHandler->fireClose(this, code, reason);
}
