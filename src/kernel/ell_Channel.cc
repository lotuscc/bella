
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
#include "ell_log.h"

#include "ell_Channel.h"

#include "ell_EventLoop.h"

// ell_Channel 不拥有 fd_ 的所有权
// 相当与挂在 fd_ 上的一条观察通道，从某一个角度观察 fd_ 上的事件
// 一个 fd_ 可以拥有多个 ell_Channel，例如，一个 ell_Channel
// 负责写，一个ell_Channel 负责读，等等

void ell_Channel::remake(std::shared_ptr<ell_EventLoop> loop, int fd) {
    loop_ = loop;
    fd_ = fd;
    events_ = 0;
    revents_ = 0;
}

ell_Channel::ell_Channel(std::shared_ptr<ell_EventLoop> loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0) {}

ell_Channel::~ell_Channel() {}

int ell_Channel::fd() const { return fd_; }

int ell_Channel::events() const { return events_; }

void ell_Channel::set_revents(int revt) { revents_ = revt; }

void ell_Channel::enableReading() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ |= kReadEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}
void ell_Channel::disableReading() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ &= ~kReadEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}

void ell_Channel::enableWriting() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ |= kWriteEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}
void ell_Channel::disableWriting() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ &= ~kWriteEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}

void ell_Channel::enableClosing() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ |= kCloseEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}
void ell_Channel::disableClosing() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ &= ~kCloseEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}

void ell_Channel::disableAll() {
    std::lock_guard<std::mutex> lk(mut_);
    events_ = kNoneEvent;
    if (loop_ != nullptr) {
        loop_->updateChannel(this);
    }
}

void ell_Channel::remove() {
    std::lock_guard<std::mutex> lk(mut_);
    if (loop_ != nullptr) {
        loop_->removeChannel(this);
    }
}

bool ell_Channel::isWriting() const { return events_ & kWriteEvent; }
bool ell_Channel::isReading() const { return events_ & kReadEvent; }

void ell_Channel::set_closeCallBack(EventCallBack callback) {
    closeCallBack_ = std::move(callback);
}
void ell_Channel::set_errorCallBack(EventCallBack callback) {
    errorCallBack_ = std::move(callback);
}
void ell_Channel::set_readCallBack(EventCallBack callback) {
    readCallBack_ = std::move(callback);
}
void ell_Channel::set_writeCallBack(EventCallBack callback) {
    writeCallBack_ = std::move(callback);
}

void ell_Channel::defaultCallBack(const std::string &s) {
    LOG("default: {}", s);
}

// 根据 revents_ 处理对应事件
void ell_Channel::handleEvent() {

    LOG("revents_: {}, fd: {}", revents_, fd_);

    // && !(revents_ & EPOLLIN)
    if ((revents_ & (EPOLLHUP | EPOLLRDHUP))) {
        LOG("closeCallBack_");
        if (closeCallBack_) {
            closeCallBack_();
        }
    }

    if (revents_ & (EPOLLERR)) {
        LOG("errorCallBack_");
        if (errorCallBack_) {
            errorCallBack_();
        }
    }
    if (revents_ & (EPOLLIN | EPOLLPRI)) {
        LOG("readCallBack_");
        if (readCallBack_) {
            readCallBack_();
        }
    }
    if (revents_ & EPOLLOUT) {
        LOG("writeCallBack_");
        if (writeCallBack_) {
            writeCallBack_();
        }
    }
}

ell_Channel &ell_Channel::from(std::shared_ptr<ell_EventLoop> loop, int fd) {
    auto channel = ell_Channel(loop, fd);
    return channel;
}