#include <cstdlib>
#include <HttpServer.hpp>
#include <App.hpp>
#include <iostream>
#include <sstream>
#include <network/io/DataOutputStream.hpp>
#include <utils/SyncTimer.hpp>

using namespace std;


int main() {
    int port = 8000;
    app.init();
    try {
        cout << "Starting server on " << port << " port" << endl;

        /* HTTP */
        HttpServer srv;
        srv.setContentRoot("share");
        srv.onNotFound([](HttpSocket *hs, http::head &head, map<string, string> &props) {
            cout << head.path << endl;
            stringstream html;
            html << ""
                    "<title>Test C++ HTTP Server</title>\n"
                    "<h1>404 Not found</h1>\n"
                    "<h2>Request headers</h2>\n"
                    "<pre>\n";
            html << '\"' << head.method << '\"' << endl;
            html << '\"' << head.path << '\"' << endl;
            html << '\"' << head.protocol << '\"' << endl;
            for(auto const &p : props) {
                html << '\"' << p.first << "\" -> \"" << p.second << '\"' << endl;
            }
            html << "</pre>\n";
            hs->sendNotFound(html.str());
            return true;
        });
        srv.onGet("/", [](HttpSocket *hs, http::head &head, map<string, string> &props) {
            uint64_t len = 0;
            char *buf = readFile("share/Index.html", &len);
            if(buf != nullptr) {
                string content = string(buf, len);
                delete[] buf;
                map<string, string> mapping = {
                        {"Host", props["Host"]},
                        {"Version", to_string(rand())}
                };
                for(auto const &e : mapping) {
                    size_t pos = 0;
                    string pattern = "$" + e.first;
                    while((pos = content.find(pattern, pos)) != std::string::npos) {
                        content.replace(pos, pattern.size(), e.second);
                    }
                }
                hs->sendContent(content);
                return true;
            }
            return false;
        });

        /* WEB SOCKET */
        WebSocketServer *wss = srv.getWebSocketServer();
        wss->onWsOpen([](WebSocket *ws) {
            cout << ws->tcp()->getAddressString() << " Соединение установлено" << endl;
//            ws->close(ws::NORMAL, "Closed gracefully");
        });
        wss->onWsMessage([](WebSocket *ws, InputStream *is) {
            const string &message = is->readRawString();
            cout << ws->tcp()->getAddressString() << " Получены данные \"" << message << "\"" << endl;
        });
        wss->onWsClose([](WebSocket *ws, int code, string reason) {
            cout << ws->tcp()->getAddressString() << " Соединение закрыто. Код: " << code << " причина: " << reason << endl;
        });
        wss->onWsError([](WebSocket *ws, IOException &e) {
            cerr << ws->tcp()->getAddressString() << " Ошибка: ";
            e.printError();
        });

        int val = rand() % 300;
        SyncTimer timer(200, [&]() {
            val += rand() % 50 - 25;
            try {
                stringstream ss;
                ss << "{\"val\": \"" << val << "\"}";
                wss->broadcastJson(ss.str());
//                ByteArrayOutputStream baos;
//                DataOutputStream dos(&baos);
//                dos.writeInteger(val);
//                wss->broadcastRaw(baos);
            } catch (IOException e) {
                cerr << "timer -> ";
                e.printError();
            }
        });
        TcpSocketServer tss(port);
        while (app.isAlive()) {
            try {
                TcpSocket *sock = tss.accept();
                if(sock != nullptr) {
                    srv.create(sock);
                }
                srv.tick();
                timer.tick();
            } catch (IOException e) {
                e.printError();
            }
        }
    } catch (RuntimeException e) {
        e.printError();
    }
    cout << "Closed gracefully" << endl;
    return EXIT_SUCCESS;
}
