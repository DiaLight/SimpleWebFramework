//
// Created by dialight on 15.04.17.
//

#include <network/protocol/WS.hpp>
#include "WebSocketServer.hpp"

WebSocketServer::~WebSocketServer() {
    for(auto const &webSocket : webSockets) {
        TcpSocket *socket = webSocket->tcp();
        delete webSocket;
        delete socket;
    }
    webSockets.clear();
}

void WebSocketServer::onWsOpen(WebSocketListener listener) {
    handlers.onOpenListeners.push_back(listener);
}

void WebSocketServer::onWsMessage(WebSocketMessageListener listener) {
    handlers.onMessageListeners.push_back(listener);
}

void WebSocketServer::onWsClose(WebSocketCloseListener listener) {
    handlers.onCloseListeners.push_back(listener);
}

void WebSocketServer::onWsError(WebSocketErrorListener listener) {
    handlers.onErrorListeners.push_back(listener);
}

void WebSocketServer::create(TcpSocket *socket) {
    WebSocket *webSocket = new WebSocket(socket, &handlers);
    webSockets.push_back(webSocket);
}

void WebSocketServer::tick() {
    for (auto iter = webSockets.begin(); iter != webSockets.end(); ) {
        WebSocket *webSocket = *iter;

        try {
            webSocket->tick();
        } catch (IOException e) {
            handlers.fireError(webSocket, e);
            try {
                webSocket->close(ws::EXCEPTION, "websocket.tick");
            } catch (IOException e) {
                e.printError();
            }
        }

        if (!webSocket->isOpen()) {
            TcpSocket *socket = webSocket->tcp();
            delete webSocket;
            delete socket;
            iter = webSockets.erase(iter);
            continue;
        }
        ++iter;
    }
}

void WebSocketServer::broadcastRaw(ByteArrayOutputStream &baos) {
    for(auto const &ws : webSockets) {
        ws->sendRaw(baos);
    }
}

void WebSocketServer::broadcastJson(std::string const &json) {
    for(auto const &ws : webSockets) {
        ws->sendJson(json);
    }
}
