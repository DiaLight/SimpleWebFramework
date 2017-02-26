//
// Created by dialight on 31.10.16.
//

#include "network/exceptions/RuntimeException.hpp"
#include <iostream>
#include <cstring>

using namespace std;

RuntimeException::RuntimeException(const string &method) : method(method), reason("errno: ") {
    reason += to_string(errno) + " " + strerror(errno);
}

RuntimeException::RuntimeException(const string &method, const string &reason) : method(method), reason(reason) {

}

RuntimeException::~RuntimeException() {

}

void RuntimeException::printError() {
    cerr << method << ". " << reason << endl;
}
