//
// Created by Light on 20.02.2017.
//

#include "network/TcpSocketServer.hpp"

#include <string.h>
#include <unistd.h>
#include <network/exceptions/IOException.hpp>
#include <fcntl.h>
#include <iostream>

using namespace std;

TcpSocketServer::TcpSocketServer(int port) : srvSocket(::socket(AF_INET, SOCK_STREAM, 0)) {
    if (srvSocket < 0) {
        throw IOException("TcpSocketServer.socket");
    }
    int option = 1;
    setsockopt(srvSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    fcntl(srvSocket, F_SETFL, O_NONBLOCK);
    bind(port);
    listen(16);
}

TcpSocketServer::~TcpSocketServer() {
    close(srvSocket);
}

void TcpSocketServer::bind(int port) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof (addr);
    memset(&addr, 0, addrLen);
    addr.sin_family = AF_UNSPEC;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t) port);

    if (::bind(srvSocket, (struct sockaddr *) &addr, addrLen) < 0) {
        throw IOException("bind");
    }
}

void TcpSocketServer::listen(int queueSize) {
    if (::listen(srvSocket, queueSize) < 0) {
        throw IOException("listen");
    }
}

TcpSocket *TcpSocketServer::accept() {
    struct sockaddr_storage clientAddr;
    socklen_t addrSize = sizeof clientAddr;

    int cliSocket = ::accept(srvSocket, (struct sockaddr *) &clientAddr, &addrSize);
    if (cliSocket < 0) {
        if(errno == EWOULDBLOCK) return nullptr;
        throw IOException("TcpSocketServer.accept");
    }
//    fcntl(cliSocket, F_SETFL, O_NONBLOCK);

    return new TcpSocket(cliSocket, clientAddr);
}
