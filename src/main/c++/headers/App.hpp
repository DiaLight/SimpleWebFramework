/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppState.h
 * Author: dialight
 *
 * Created on 26 октября 2016 г., 17:46
 */

#ifndef APPSTATE_H__
#define APPSTATE_H__

#include <atomic>
#include <condition_variable>
#include <vector>

class App;
extern App app;

class App {
    std::atomic<bool> alive;
    std::atomic<bool> notified;
    std::mutex m;
    std::condition_variable condv;
    std::vector<std::function<void()>> onCloseListeners;
public:
    App();

    virtual ~App();

    void wait(int seconds);
    void waitms(int milliseconds);
    void notifyAll();

    void close();

    bool isAlive();

    void init();

    void onClose(std::function<void()> lambda);
};

#endif /* APPSTATE_H */

