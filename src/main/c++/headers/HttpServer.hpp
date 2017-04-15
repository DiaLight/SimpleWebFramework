//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP
#define SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP

#include <network/TcpSocketServer.hpp>
#include <network/nonblocking/WebSocket.hpp>
#include <vector>
#include <network/nonblocking/HttpSocket.hpp>
#include "WebSocketServer.hpp"

class HttpServer {

    HttpHandler handler;
    std::vector<HttpSocket *> sockets;
    WebSocketServer wss;
public:

    virtual ~HttpServer();

    void onGet(std::string path, HttpPacketHandler p);

    void create(TcpSocket *socket);

    void tick();

    WebSocketServer *getWebSocketServer() {
        return &wss;
    }

    void setContentRoot(std::string const &root);

    void onNotFound(HttpPacketHandler p);

};


#endif //SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP
