//
// Created by dialight on 31.10.16.
//

#ifndef OPENCVCLIENT_RUNTIMEEXCEPTION_HPP
#define OPENCVCLIENT_RUNTIMEEXCEPTION_HPP

#include <string>

class RuntimeException {
    std::string method;
    std::string reason;
public:
    RuntimeException(const std::string &method);
    RuntimeException(const std::string &method, const std::string &reason);
    //RuntimeException(const RuntimeException&) = delete; //deleted copy constructor
    RuntimeException& operator=(const RuntimeException &) = delete; //deleted copy assignment operato

    virtual ~RuntimeException();

    void printError();
};


#endif //OPENCVCLIENT_RUNTIMEEXCEPTION_HPP
