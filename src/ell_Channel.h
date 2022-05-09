
#pragma once

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <poll.h>
#include <string>
#include <sys/epoll.h>
#include <vector>

// #include "ell_EventLoop.hpp"
#include "ell_log.hpp"

// ell_Channel 不拥有 fd_ 的所有权
// 相当与挂在 fd_ 上的一条观察通道，从某一个角度观察 fd_ 上的事件
// 一个 fd_ 可以拥有多个 ell_Channel，例如，一个 ell_Channel
// 负责写，一个ell_Channel 负责读，等等

// 前向声明
class ell_EventLoop;

class ell_Channel {

public:
    using EventCallBack = std::function<void()>;
    using ChannelList = std::vector<ell_Channel *>;

private:
    static const int kNoneEvent = 0;
    static const int kReadEvent = EPOLLIN;
    static const int kWriteEvent = EPOLLOUT;
    static const int kCloseEvent = EPOLLRDHUP;

    ell_EventLoop *loop_;

    const int fd_;
    int events_;
    int revents_;

    // void sayhello() { std::cout << "hello in Channel" << std::endl; }

    EventCallBack closeCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "close");
    EventCallBack errorCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "error");
    EventCallBack writeCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "write");
    EventCallBack readCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "read");

public:
    ~ell_Channel();
    ell_Channel(ell_EventLoop *loop, int fd);
    ell_Channel &from(ell_EventLoop *loop, int fd);

    ell_Channel(const ell_Channel &) = delete;
    ell_Channel &operator=(const ell_Channel &) = delete;

    void handleEvent();

    int fd() const;
    int events() const;

    // used by pollers
    void set_revents(int revt);

    void enableReading();
    void disableReading();

    void enableWriting();
    void disableWriting();

    void enableClosing();
    void disableClosing();

    void disableAll();

    bool isWriting() const;
    bool isReading() const;

    void set_closeCallBack(EventCallBack callback);
    void set_errorCallBack(EventCallBack callback);
    void set_readCallBack(EventCallBack callback);
    void set_writeCallBack(EventCallBack callback);

    void defaultCallBack(const std::string &s);
};