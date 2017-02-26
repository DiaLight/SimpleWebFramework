#ifndef SOCKET_WEBSERVER_TCPSOCKETSERVER_H
#define SOCKET_WEBSERVER_TCPSOCKETSERVER_H

#include "TcpSocket.hpp"

class TcpSocketServer {
    int srvSocket;
public:

    TcpSocketServer(int port);

    virtual ~TcpSocketServer();
    TcpSocketServer(const TcpSocketServer&) = delete; //deleted copy constructor
    TcpSocketServer& operator=(const TcpSocketServer &) = delete; //deleted copy assignment operato

    TcpSocket *accept();

private:
    void bind(int port);
    void listen(int queueSize);

};


#endif //SOCKET_WEBSERVER_TCPSOCKETSERVER_H
