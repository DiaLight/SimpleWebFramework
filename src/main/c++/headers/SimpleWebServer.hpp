//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP
#define SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP

#include <network/TcpSocketServer.hpp>
#include <network/nonblocking/WebSocket.hpp>
#include <vector>
#include <network/nonblocking/HttpSocket.hpp>

class SimpleWebServer : public TcpSocketServer {


    //event based system
    HttpHandler httpHandler;
    WebSocketHandler wsHandler;
public:
    //core
    std::vector<HttpSocket *> httpSockets;
    std::vector<WebSocket *> webSockets;

    SimpleWebServer(int port);

    virtual ~SimpleWebServer();

    void tick();

    HttpHandler &getHttpHandler();

    WebSocketHandler &getWebSocketHandler();
};


#endif //SOCKET_WEBSERVER_SIMPLEWEBSERVER_HPP
