//
// Created by dialight on 21.02.17.
//

#include "network/TcpSocket.hpp"

#include <unistd.h>
#include <sstream>

using namespace std;

void *getInAddr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) return &(((struct sockaddr_in *) sa)->sin_addr);
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

in_port_t getPort(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) return ((struct sockaddr_in *) sa)->sin_port;
    return ((struct sockaddr_in6 *) sa)->sin6_port;
}

TcpSocket::TcpSocket(int socket, struct sockaddr_storage addr) : socket(socket), addr(addr), is(socket), os(socket) {
}

TcpSocket::~TcpSocket() {
    ::close(socket);
}

string TcpSocket::getAddressString() {
    struct sockaddr *saddr = (struct sockaddr *) &addr;
    static char s[INET6_ADDRSTRLEN];
    inet_ntop(saddr->sa_family, getInAddr(saddr), s, INET6_ADDRSTRLEN);
    stringstream ss;
    ss << s << ":" << getPort(saddr);
    return ss.str();
}

InputStream *TcpSocket::getInputStream() {
    return &is;
}

OutputStream *TcpSocket::getOutputStream() {
    return &os;
}
