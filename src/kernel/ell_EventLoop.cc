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

#include "ell_EventLoop.h"

void ell_EventLoop::loop() {
    while (true) {
        activeChannels_.clear();

        // 阻塞
        poller_->poll(-1, &activeChannels_);

        for (ChannelList::iterator it = activeChannels_.begin();
             it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
            // 可以优化为异步执行

        }
    }
}

void ell_EventLoop::appendChannel(ell_Channel *channel) {
    poller_->append_listenChannel(channel);
}

void ell_EventLoop::removeChannel(ell_Channel *channel) {
    poller_->remove_listenChannel(channel);
}

void ell_EventLoop::updateChannel(ell_Channel *channel) {
    poller_->updateChannel(channel);
}

ell_EventLoop::ell_EventLoop() { poller_ = std::make_unique<ell_EPoller>(); }

ell_EventLoop::~ell_EventLoop() {}
