//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_TCPSOCKET_HPP
#define SOCKET_WEBSERVER_TCPSOCKET_HPP

#include <arpa/inet.h>
#include <network/io/base/InputStream.hpp>
#include <network/io/base/OutputStream.hpp>
#include <string>
#include <network/io/base/SocketInputStream.hpp>
#include <network/io/base/SocketOutputStream.hpp>

class TcpSocket {
    int socket;
    struct sockaddr_storage addr;
    SocketInputStream is;
    SocketOutputStream os;
public:
    TcpSocket(int socket, struct sockaddr_storage addr);
    TcpSocket(const TcpSocket&) = delete; //deleted copy constructor
    TcpSocket& operator=(const TcpSocket &) = delete; //deleted copy assignment operato

    virtual ~TcpSocket();

    std::string getAddressString();

    InputStream *getInputStream();

    OutputStream *getOutputStream();

};


#endif //SOCKET_WEBSERVER_TCPSOCKET_HPP
