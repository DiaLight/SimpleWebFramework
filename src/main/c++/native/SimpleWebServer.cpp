//
// Created by dialight on 21.02.17.
//

#include "SimpleWebServer.hpp"

#include <network/protocol/WS.hpp>

using namespace std;

SimpleWebServer::SimpleWebServer(int port) : TcpSocketServer(port) {}


SimpleWebServer::~SimpleWebServer() {
    for(auto const &httpSocket : httpSockets) {
        TcpSocket *socket = httpSocket->tcp();
        delete httpSocket;
        delete socket;
    }
    httpSockets.clear();
    for(auto const &webSocket : webSockets) {
        TcpSocket *socket = webSocket->tcp();
        delete webSocket;
        delete socket;
    }
    webSockets.clear();
}

void SimpleWebServer::tick() {
    try {
        TcpSocket *sock = accept();
        if(sock != nullptr) {
            httpSockets.push_back(new HttpSocket(sock, &httpHandler));
        }
    } catch (IOException e) {
        e.printError();
    }

    for (auto iter = httpSockets.begin(); iter != httpSockets.end(); ) {
        HttpSocket *&httpSocket = *iter;

        try {
            httpSocket->tick();
        } catch (IOException e) {
            e.printError();
            httpSocket->close();
        }

        if (httpSocket->isUpgrade()) {
            iter = httpSockets.erase(iter);

            TcpSocket *socket = httpSocket->tcp();
            delete httpSocket;

            WebSocket *webSocket = new WebSocket(socket, &wsHandler);
            webSockets.push_back(webSocket);
            continue;
        }
        if(!httpSocket->isOpen()){
            TcpSocket *socket = httpSocket->tcp();
            delete httpSocket;
            delete socket;
            iter = httpSockets.erase(iter);
            continue;
        }
        ++iter;
    }

    for (auto iter = webSockets.begin(); iter != webSockets.end(); ) {
        WebSocket *&webSocket = *iter;

        try {
            webSocket->tick();
        } catch (IOException e) {
            wsHandler.fireError(webSocket, e);
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

HttpHandler &SimpleWebServer::getHttpHandler() {
    return httpHandler;
}

WebSocketHandler &SimpleWebServer::getWebSocketHandler() {
    return wsHandler;
}

