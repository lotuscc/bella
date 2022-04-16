#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <vector>

#include "ell_Channel.hpp"
#include "ell_EPoller.hpp"

using ChannelList = ell_Channel::ChannelList;

class ell_EventLoop {

    ChannelList activeChannels_;

public:
    ell_EventLoop();
    ~ell_EventLoop();
    ell_EventLoop(const ell_EventLoop &) = delete;
    ell_EventLoop &operator=(const ell_EventLoop &) = delete;

    void loop();

    std::unique_ptr<ell_EPoller> poller_;

    void append_channel(ell_Channel *channel);
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
    poller_->update(0, channel);
}

ell_EventLoop::ell_EventLoop() { poller_ = std::make_unique<ell_EPoller>(); }

ell_EventLoop::~ell_EventLoop() {}
