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

#define MAX_BUF_SIZE 1024

class HttpSocket;
typedef bool(*HttpPacketHandler)(HttpSocket *httpSocket, http::head &head, std::map<std::string, std::string> &props);


class HttpHandler {
    std::map<std::string, HttpPacketHandler> httpHandlers;
    std::string root = "";
public:
    HttpHandler();

    HttpHandler(const HttpHandler&) = delete; //deleted copy constructor
    HttpHandler& operator=(const HttpHandler &) = delete; //deleted copy assignment operator
    HttpPacketHandler notFound = nullptr;

    void setContentRoot(std::string const &root) {
        if(root[root.size() - 1] == '/') {
            this->root = root;
        } else {
            this->root = root + '/';
        }
    }

    void onGet(std::string path, HttpPacketHandler p);

    void handleHttp(HttpSocket *httpSocket, http::head &head, std::map<std::string, std::string> &props);

    static char *readFile(std::string const &path, uint64_t *length);
};

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

    void sendContent(ByteArrayOutputStream *baos);
    void sendContent(std::string const &content);

    void sendNotFound(std::string const &content);

};


#endif //SOCKET_WEBSERVER_HTTPSOCKET_HPP
