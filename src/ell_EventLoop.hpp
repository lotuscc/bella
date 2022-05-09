#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <vector>

#include "ell_Channel.h"
#include "ell_EPoller.h"

// class ell_Channel;
// class ell_EPoller;

// using ChannelList = ell_Channel::ChannelList;

class ell_EventLoop {

    using ChannelList = std::vector<ell_Channel *>;

    ChannelList activeChannels_;
    std::unique_ptr<ell_EPoller> poller_;

public:
    ell_EventLoop();
    ~ell_EventLoop();
    ell_EventLoop(const ell_EventLoop &) = delete;
    ell_EventLoop &operator=(const ell_EventLoop &) = delete;

    void loop();
    void append_channel(ell_Channel *channel);
    void remove_Channel(ell_Channel *channel);
    void update_Channel(ell_Channel *channel);

    void make();
};

void ell_EventLoop::loop() {
    while (true) {
        activeChannels_.clear();

        // 阻塞
        poller_->poll(-1, &activeChannels_);

        for (ChannelList::iterator it = activeChannels_.begin();
             it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void ell_EventLoop::append_channel(ell_Channel *channel) {
    poller_->append_listenChannel(channel);
}

void ell_EventLoop::remove_Channel(ell_Channel *channel) {
    poller_->remove_listenChannel(channel);
}

void ell_EventLoop::update_Channel(ell_Channel *channel) {
    poller_->updateChannel(channel);
}

ell_EventLoop::ell_EventLoop() { poller_ = std::make_unique<ell_EPoller>(); }

ell_EventLoop::~ell_EventLoop() {}
