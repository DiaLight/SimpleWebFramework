//
// Created by dialight on 21.02.17.
//

#ifndef SOCKET_WEBSERVER_HTTPPARSEEXCEPTION_HPP
#define SOCKET_WEBSERVER_HTTPPARSEEXCEPTION_HPP

#include <network/exceptions/RuntimeException.hpp>

class ParseException : public RuntimeException {

public:
    ParseException(const std::string &method) : RuntimeException(method) {}

    ParseException(const std::string &method, const std::string &reason) : RuntimeException(method, reason) {}

};


#endif //SOCKET_WEBSERVER_HTTPPARSEEXCEPTION_HPP
