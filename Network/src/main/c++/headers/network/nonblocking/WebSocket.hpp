//
// Created by dialight on 22.02.17.
//

#ifndef SOCKET_WEBSERVER_WEBSOCKET_HPP
#define SOCKET_WEBSERVER_WEBSOCKET_HPP


#include <network/TcpSocket.hpp>
#include <network/io/base/ByteArrayOutputStream.hpp>
#include <vector>
#include <network/exceptions/IOException.hpp>

class WebSocket;

typedef void (*WebSocketListener)(WebSocket *);
typedef void (*WebSocketMessageListener)(WebSocket *, InputStream *);
typedef void (*WebSocketCloseListener)(WebSocket *, int code, std::string reason);
typedef void (*WebSocketErrorListener)(WebSocket *, IOException &e);

class WebSocketHandler {
    std::vector<WebSocketListener> onOpenListeners;
    std::vector<WebSocketMessageListener> onMessageListeners;
    std::vector<WebSocketCloseListener> onCloseListeners;
    std::vector<WebSocketErrorListener> onErrorListeners;

public:
    void onWsOpen(WebSocketListener listener);

    void onWsMessage(WebSocketMessageListener listener);

    void onWsClose(WebSocketCloseListener listener);

    void onWsError(WebSocketErrorListener listener);

    void fireOpen(WebSocket *webSocket);

    void fireMessage(WebSocket *webSocket, InputStream *message);

    void fireClose(WebSocket *webSocket, int code, std::string const &reason);

    void fireError(WebSocket *webSocket, IOException &e);
};

class WebSocket {

    TcpSocket *socket;
    unsigned long lastLen;
    bool opened = true;
    WebSocketHandler *wsHandler;
public:
    WebSocket(TcpSocket *socket, WebSocketHandler *wsHandler);

    virtual ~WebSocket();

    void tick();

    TcpSocket *tcp();

    bool isOpen();

    void close(int code, std::string const &reason);

    void sendMessage(ByteArrayOutputStream &baos);

};


#endif //SOCKET_WEBSERVER_WEBSOCKET_HPP
