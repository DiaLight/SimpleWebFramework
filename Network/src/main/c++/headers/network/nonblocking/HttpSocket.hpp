//
// Created by dialight on 23.02.17.
//

#ifndef SOCKET_WEBSERVER_HTTPSOCKET_HPP
#define SOCKET_WEBSERVER_HTTPSOCKET_HPP

#include <network/exceptions/IOException.hpp>
#include <map>
#include <network/protocol/HTTP.hpp>
#include <network/TcpSocket.hpp>
#include <network/io/base/ByteArrayOutputStream.hpp>
#include <fstream>

#define MAX_BUF_SIZE 1024

char *readFile(std::string const &path, uint64_t *length);

class HttpSocket;
typedef bool(*HttpPacketHandler)(HttpSocket *httpSocket, http::head &head, std::map<std::string, std::string> &props);

typedef struct _HttpHandler {
    std::map<std::string, HttpPacketHandler> httpHandlers;
    std::string root = "";
    HttpPacketHandler notFound = nullptr;

    void handleHttp(HttpSocket *httpSocket, http::head &head, std::map<std::string, std::string> &props);

} HttpHandler;

class HttpSocket {
    HttpHandler *httpHandler;
    TcpSocket *socket;
    char buf[MAX_BUF_SIZE];
    bool upgrade = false;
    bool open = true;
public:
    HttpSocket(TcpSocket *socket, HttpHandler *httpHandler);
    HttpSocket(const HttpSocket&) = delete; //deleted copy constructor
    HttpSocket& operator=(const HttpSocket &) = delete; //deleted copy assignment operator
    virtual ~HttpSocket();

    void tick();

    TcpSocket *tcp();

    bool isUpgrade();
    bool isOpen();
    void close();

    void sendContent(ByteArrayOutputStream *baos, std::string const &type);
    void sendContent(std::string const &content);

    void sendNotFound(std::string const &content);

};


#endif //SOCKET_WEBSERVER_HTTPSOCKET_HPP
