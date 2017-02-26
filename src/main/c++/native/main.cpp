#include <cstdlib>
#include <SimpleWebServer.hpp>
#include <App.hpp>
#include <iostream>
#include <sstream>
#include <network/protocol/WS.hpp>
#include <network/io/DataOutputStream.hpp>
#include <utils/NBTimer.hpp>

using namespace std;


int main() {
    app.init();
    try {
        SimpleWebServer srv(8888);
        HttpHandler &httpHandler = srv.getHttpHandler();
        httpHandler.setContentRoot("share");
        httpHandler.notFound = [](HttpSocket *hs, http::head &head, map<string, string> &props) {
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
        };
        httpHandler.onGet("/", [](HttpSocket *hs, http::head &head, map<string, string> &props) {
            uint64_t len = 0;
            char *buf = HttpHandler::readFile("Index.html", &len);
            if(buf != nullptr) {
                string content = string(buf, len);
                delete[] buf;

                size_t pos;
                static string const &host = "$Host";
                pos = 0;
                while((pos = content.find(host, pos)) != std::string::npos) {
                    content.replace(pos, host.size(), props["Host"]);
                }
                string curVer = to_string(rand());
                static string const &version = "$Version";
                pos = 0;
                while((pos = content.find(version, pos)) != std::string::npos) {
                    content.replace(pos, version.size(), curVer);
                }
                hs->sendContent(content);
                return true;
            }
            return false;
        });

        WebSocketHandler &wsHandler = srv.getWebSocketHandler();
        wsHandler.onWsOpen([](WebSocket *ws) {
            cout << ws->tcp()->getAddressString() << " Соединение установлено" << endl;
//            ws->close(ws::NORMAL, "Closed gracefully");
        });
        wsHandler.onWsMessage([](WebSocket *ws, InputStream *is) {
            const string &message = is->readRawString();
            cout << ws->tcp()->getAddressString() << " Получены данные \"" << message << "\"" << endl;
        });
        wsHandler.onWsClose([](WebSocket *ws, int code, string reason) {
            cout << ws->tcp()->getAddressString() << " Соединение закрыто. Код: " << code << " причина: " << reason << endl;
        });
        wsHandler.onWsError([](WebSocket *ws, IOException &e) {
            cerr << ws->tcp()->getAddressString() << " Ошибка: ";
            e.printError();
        });

        NBTimer timer(200 * 1000 * 1000);
        int val = rand();
        while (app.isAlive()) {
            srv.tick();
            if(timer.point()) {
//                cout << '.' << flush;
                val += rand() % 50 - 25;
                try {
                    for(auto const &ws : srv.webSockets) {
                        ByteArrayOutputStream baos;
                        DataOutputStream dos(&baos);
                        dos.writeInteger(val);
                        ws->sendMessage(baos);
                    }
                } catch (IOException e) {
                    cerr << "timer -> ";
                    e.printError();
                }
            }
        }
    } catch (RuntimeException e) {
        e.printError();
    }
    cout << "Closed gracefully" << endl;
    return EXIT_SUCCESS;
}
