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

typedef struct _WebSocketHandlers {
    std::vector<WebSocketListener> onOpenListeners;
    std::vector<WebSocketMessageListener> onMessageListeners;
    std::vector<WebSocketCloseListener> onCloseListeners;
    std::vector<WebSocketErrorListener> onErrorListeners;

    void fireOpen(WebSocket *webSocket) {
        for(auto const &listener : onOpenListeners) listener(webSocket);
    }

    void fireMessage(WebSocket *webSocket, InputStream *message) {
        for(auto const &listener : onMessageListeners) listener(webSocket, message);
    }

    void fireClose(WebSocket *webSocket, int code, std::string const &reason) {
        for(auto const &listener : onCloseListeners) listener(webSocket, code, reason);
    }

    void fireError(WebSocket *webSocket, IOException &e) {
        for(auto const &listener : onErrorListeners) listener(webSocket, e);
    }

} WebSocketHandlers;

class WebSocket {

    TcpSocket *socket;
    bool opened = true;
    WebSocketHandlers *handlers;
public:
    WebSocket(TcpSocket *socket, WebSocketHandlers *handlers);

    virtual ~WebSocket();

    void tick();

    TcpSocket *tcp();

    bool isOpen();

    void close(int code, std::string const &reason);

    void sendJson(std::string const &str);

    void sendRaw(ByteArrayOutputStream &baos);

};


#endif //SOCKET_WEBSERVER_WEBSOCKET_HPP
