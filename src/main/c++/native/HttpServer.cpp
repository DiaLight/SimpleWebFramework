//
// Created by dialight on 21.02.17.
//

#include "HttpServer.hpp"

#include <network/protocol/WS.hpp>

using namespace std;

HttpServer::~HttpServer() {
    for(auto const &httpSocket : sockets) {
        TcpSocket *socket = httpSocket->tcp();
        delete httpSocket;
        delete socket;
    }
    sockets.clear();
}

void HttpServer::create(TcpSocket *socket) {
    sockets.push_back(new HttpSocket(socket, &handler));
}

void HttpServer::onGet(string path, HttpPacketHandler p) {
    handler.httpHandlers.insert(pair<string, HttpPacketHandler>(path, p));
}

void HttpServer::tick() {

    for (auto iter = sockets.begin(); iter != sockets.end(); ) {
        HttpSocket *httpSocket = *iter;

        try {
            httpSocket->tick();
        } catch (IOException e) {
            e.printError();
            httpSocket->close();
        }

        if (httpSocket->isUpgrade()) {
            iter = sockets.erase(iter);

            TcpSocket *socket = httpSocket->tcp();
            delete httpSocket;

            wss.create(socket);
            continue;
        }
        if(!httpSocket->isOpen()){
            TcpSocket *socket = httpSocket->tcp();
            delete httpSocket;
            delete socket;
            iter = sockets.erase(iter);
            continue;
        }
        ++iter;
    }

    wss.tick();
}

void HttpServer::setContentRoot(string const &root) {
    if(root[root.size() - 1] == '/') {
        handler.root = root;
    } else {
        handler.root = root + '/';
    }
}

void HttpServer::onNotFound(HttpPacketHandler p) {
    handler.notFound = p;
}

