//
// Created by dialight on 06.11.16.
//
#include <App.hpp>
#include <iostream>
#include <csignal>
#include <string.h>

using namespace std;
using namespace std::chrono;

App app;

App::App() : notified(false), alive(true) {}

void App::wait(int sec) {
    unique_lock<mutex> lk(m);
    notified = false;
    condv.wait_until(lk, system_clock::now() + seconds(sec), [&](){return notified == true;});
}

void App::waitms(int ms) {
    unique_lock<mutex> lk(m);
    notified = false;
    condv.wait_until(lk, system_clock::now() + milliseconds(ms), [&](){return notified == true;});
}

void App::notifyAll() {
    unique_lock<mutex> lk(m);
    notified = true;
    condv.notify_all();
}

void App::close() {
    alive = false;
    for(auto const &listener : onCloseListeners) {
        listener();
    }
    notifyAll();
}

bool App::isAlive() {
    return alive;
}

void signalHandler(int signum) {
    app.close();
    cout << "Terminated with signal " << signum << " " << strsignal(signum) << endl;
}

void App::init() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

App::~App() {
//    cout << "Reset signal handlers to default";
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

void App::onClose(std::function<void()> lambda) {
    onCloseListeners.push_back(lambda);
}
