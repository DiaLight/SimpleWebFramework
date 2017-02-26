//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_HTTP_HPP
#define SOCKET_WEBSERVER_HTTP_HPP

#include <string>
#include <map>
#include "network/exceptions/ParseException.hpp"

namespace http {

    typedef struct head_t {
        std::string method;
        std::string path;
        std::string protocol;
    } head;

    void parse(char *buf, head &head, std::map<std::string, std::string> &props);

};


#endif //SOCKET_WEBSERVER_HTTP_HPP
