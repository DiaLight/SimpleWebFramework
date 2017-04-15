//
// Created by dialight on 23.02.17.
//
#include "network/nonblocking/HttpSocket.hpp"

#include <network/crypto/base64.hpp>
#include <network/crypto/sha1.hpp>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using namespace std;

HttpSocket::HttpSocket(TcpSocket *socket, HttpHandler *httpHandler) : socket(socket), httpHandler(httpHandler) {}

HttpSocket::~HttpSocket() {

}

void HttpSocket::tick() {

    InputStream *is = socket->getInputStream();
    OutputStream *os = socket->getOutputStream();

    uint64_t len = is->read(buf, 0, MAX_BUF_SIZE);
    if(len == 0) return;
    buf[len] = '\0';

    http::head h;
    map<string, string> props;
    try {
        http::parse((char *) buf, h, props);
    } catch (ParseException e) {
        e.printError();
        cerr << "dump:" << endl << buf << endl << endl;
        return;
    }

//    cout << socket->getAddressString() << endl;
//    for(auto const &e : props) {
//        cout << e.first << ": " << e.second << endl;
//    }
//    cout << endl;

    if (props["Connection"].find("Upgrade") != string::npos && props["Upgrade"] == "websocket") {
        //handshake

//        string userAgent = props["User-Agent"];
//        string extensions = props["Sec-WebSocket-Extensions"];
//        string protocol = props["Sec-WebSocket-Protocol"];
//        int version = atoi(props["Sec-WebSocket-Version"].c_str());

        const string magick = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        string handshake = props["Sec-WebSocket-Key"] + magick;
        unsigned char hash[20];
        sha1::calc(handshake.c_str(), handshake.size(), hash);

        stringstream response;
        response << ""
                "HTTP/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
//                "Sec-WebSocket-Protocol: " << props["Sec-WebSocket-Protocol"] << "\r\n"
                "Sec-WebSocket-Accept: " << base64::encode(hash, 20) << "\r\n"
                "\r\n";
        os->writeRawString(response.str());
        upgrade = true;
    } else {
        httpHandler->handleHttp(this, h, props);
        open = false;
    }
}

void HttpSocket::sendContent(ByteArrayOutputStream *baos, string const &type) {
    OutputStream *os = socket->getOutputStream();
    stringstream response;
    response << ""
            "HTTP/1.1 200 OK\r\n"
            "Version: HTTP/1.1\r\n"
            "Content-Type: " << type << "; charset=utf-8\r\n"
            "Content-Length: " << baos->size() << "\r\n"
            "\r\n";
    os->writeRawString(response.str());
    os->write(baos->getBuffer(), baos->size());
}

void HttpSocket::sendContent(std::string const &content) {
    OutputStream *os = socket->getOutputStream();
    stringstream response;
    response << ""
            "HTTP/1.1 200 OK\r\n"
            "Version: HTTP/1.1\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: " << content.size() << "\r\n"
            "\r\n";
    response << content;
    os->writeRawString(response.str());
}

void HttpSocket::sendNotFound(std::string const &content) {
    OutputStream *os = socket->getOutputStream();
    stringstream response;
    response << ""
            "HTTP/1.1 404 NOT FOUND\r\n"
            "Version: HTTP/1.1\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: " << content.size() << "\r\n"
                     "\r\n";
    response << content;
    os->writeRawString(response.str());
}

TcpSocket *HttpSocket::tcp() {
    return socket;
}

bool HttpSocket::isUpgrade() {
    return upgrade;
}

bool HttpSocket::isOpen() {
    return open;
}

void HttpSocket::close() {
    open = false;
}

bool hasEnding(string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

const char *getType(string const &path) {
    if(hasEnding(path, ".css")) return "text/css";
    if(hasEnding(path, ".js")) return "application/javascript";
    if(hasEnding(path, ".html")) return "text/html";
    return "text/plain";
}

void _HttpHandler::handleHttp(HttpSocket *httpSocket, http::head &head, std::map<std::string, std::string> &props) {
    auto entry = httpHandlers.find(head.path);
    if(entry != httpHandlers.end()) {
        if(entry->second(httpSocket, head, props)) return;
    }
    uint64_t length = 0;
    string path = head.path[0] == '/' ? root + "." + head.path : root + head.path;
    size_t pos = path.find('?');
    if(pos != string::npos) {
        path = path.substr(0, pos);
    }
    char *buf = readFile(path, &length);
    if(buf != nullptr) {
        ByteArrayOutputStream baos(buf, length);
        string type = getType(head.path);
        httpSocket->sendContent(&baos, type);
        return;
    }

    if(notFound == nullptr || !notFound(httpSocket, head, props)) {
        httpSocket->sendNotFound(head.path);
    }
}

bool is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

char *readFile(std::string const &path, uint64_t *length) {
    if(!is_regular_file(path.c_str())) return nullptr;
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if(!ifs.is_open()) return nullptr;

    std::ifstream::pos_type pos = ifs.tellg();

    char *buf = new char[pos];
    ifs.seekg(0, std::ios::beg);
    ifs.read(buf, pos);

    ifs.close();
    *length = (uint64_t) pos;
    return buf;
}
