//
// Created by dialight on 22.02.17.
//

#include <network/protocol/HTTP.hpp>
#include <cstring>

using namespace std;

namespace http {

    string toString(char *p1, char *p2, const string &pos) {
        auto len = p2 - p1;
        if(len <= 0) {
            throw ParseException("http::parse." + pos + ".toString", "bad length " + to_string(len) + "\nreverse:\"" + string(p2, p1 - p2) + "\"");
        };
        return string(p1, len);
    }

    void parse(char *buf, head &head, map<string, string> &props) {
        char *end;
        /* Must have a \r\n\r\n */
        if ((end = strstr(buf, "\r\n\r\n")) == nullptr) throw ParseException("http::parse", "incomplete http header");

        //first line
        char *req = strchr(buf, ' ');
        if(req == nullptr || (req - buf) > 6) throw ParseException("http::parse.1stLine", "unknown method " + string(buf, req - buf));
        head.method = toString(buf, req, "method");
        char *proto = strstr(req, " HTTP/");
        if (proto == nullptr) throw ParseException("http::parse.head.protocol", "unknown protocol");
        head.path = toString(req + 1, proto, "path");
        char *endl = strstr(proto + 1, "\r\n");
        if(endl == nullptr) throw ParseException("http::parse.head.endl", "end of line not found");
        head.protocol = toString(proto + 1, endl, "protocol");

        char *next = nullptr, *p;
        char *line = endl + 2;
        int len;
        while (line) {
            if ((next = strstr(line, "\r\n")) != nullptr) {
                len = (next - line);
            } else {
                len = strlen(line);
            }

            if (len <= 0) throw ParseException("http::parse.header", "bad length");

            if ((p = strchr(line, ':')) == nullptr) throw ParseException("http::parse.props", "':' not found");
            props[toString(line, p, "key")] = toString(p + 2, next, "value");

            line = next ? (next + 2) : nullptr;

            if (next >= end) break;
        }
    }
}
