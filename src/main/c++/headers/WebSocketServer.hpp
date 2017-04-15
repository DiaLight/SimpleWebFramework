//
// Created by dialight on 15.04.17.
//

#ifndef SIMPLEWEBFRAMEWORK_WEBSOCKETSERVER_HPP
#define SIMPLEWEBFRAMEWORK_WEBSOCKETSERVER_HPP

#include <vector>
#include <network/exceptions/IOException.hpp>
#include <network/nonblocking/WebSocket.hpp>

class WebSocketServer {
    WebSocketHandlers handlers;
    std::vector<WebSocket *> webSockets;
public:
    virtual ~WebSocketServer();

    void onWsOpen(WebSocketListener listener);

    void onWsMessage(WebSocketMessageListener listener);

    void onWsClose(WebSocketCloseListener listener);

    void onWsError(WebSocketErrorListener listener);

    void create(TcpSocket *socket);

    void tick();

    void broadcastRaw(ByteArrayOutputStream &stream);

    void broadcastJson(std::string const &json);
};


#endif //SIMPLEWEBFRAMEWORK_WEBSOCKETSERVER_HPP
