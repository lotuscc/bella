
#pragma once

#include <assert.h>
#include <functional>
#include <iostream>
#include <poll.h>
#include <string>
#include <vector>

// ell_Channel 不拥有 fd_ 的所有权
// 相当与挂在 fd_ 上的一条观察通道，从某一个角度观察 fd_ 上的事件
// 一个 fd_ 可以拥有多个 ell_Channel，例如，一个 ell_Channel
// 负责写，一个ell_Channel 负责读，等等

class ell_Channel {

public:
    using EventCallBack = std::function<void()>;
    using ChannelList = std::vector<ell_Channel *>;

private:
    static const int kNoneEvent = 0;
    static const int kReadEvent = POLLIN;
    static const int kWriteEvent = POLLOUT;

    const int fd_;
    int events_;
    int revents_;

    void update();

    void sayhello() { std::cout << "hello in Channel" << std::endl; }

    EventCallBack closeCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "close");
    EventCallBack errorCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "error");
    EventCallBack writeCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "write");
    EventCallBack readCallBack_ =
        std::bind(&ell_Channel::defaultCallBack, this, "read");

public:
    ell_Channel(int fd__);
    ~ell_Channel();

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

    void disableAll();

    bool isWriting() const;
    bool isReading() const;

    void set_closeCallBack(EventCallBack callback);
    void set_errorCallBack(EventCallBack callback);
    void set_readCallBack(EventCallBack callback);
    void set_writeCallBack(EventCallBack callback);

    void defaultCallBack(const std::string &s);
};

ell_Channel::ell_Channel(int fd__) : fd_(fd__), events_(0), revents_(0) {}

ell_Channel::~ell_Channel() {}

int ell_Channel::fd() const { return fd_; }

int ell_Channel::events() const { return events_; }
void ell_Channel::set_revents(int revt) { revents_ = revt; }

void ell_Channel::enableReading() { events_ |= kReadEvent; }
void ell_Channel::disableReading() { events_ &= ~kReadEvent; }

void ell_Channel::enableWriting() { events_ |= kWriteEvent; }
void ell_Channel::disableWriting() { events_ &= ~kWriteEvent; }

void ell_Channel::disableAll() { events_ = kNoneEvent; }

bool ell_Channel::isWriting() const { return events_ & kWriteEvent; }
bool ell_Channel::isReading() const { return events_ & kReadEvent; }

void ell_Channel::set_closeCallBack(EventCallBack callback) {
    closeCallBack_ = callback;
}
void ell_Channel::set_errorCallBack(EventCallBack callback) {
    errorCallBack_ = callback;
}
void ell_Channel::set_readCallBack(EventCallBack callback) {
    readCallBack_ = callback;
    std::cout << "set read" << std::endl;
}
void ell_Channel::set_writeCallBack(EventCallBack callback) {
    writeCallBack_ = callback;
}

void ell_Channel::defaultCallBack(const std::string &s) {
    std::cout << s << std::endl;
}

// 根据 revents_ 处理
void ell_Channel::handleEvent() {

    std::cout << "fd: " << fd_ << std::endl;

    if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        if (closeCallBack_) {
            closeCallBack_();
        }
    }

    if (revents_ & POLLNVAL) {
        std::cout << "fd = " << fd_ << " Channel::handle_event() POLLNVAL"
                  << std::endl;
    }

    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallBack_) {
            errorCallBack_();
        }
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallBack_) {
            readCallBack_();
        }
    }
    if (revents_ & POLLOUT) {
        std::cout << "write" << std::endl;
        static int x = 1;
        if (x == 1) {
            if (writeCallBack_) {
                ++x;
                writeCallBack_();
            }
        }
    }
}