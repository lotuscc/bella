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

#include "ell_EventLoop.h"

void ell_EventLoop::loop() {
    while (true) {
        activeChannels_.clear();

        // 阻塞
        poller_->poll(-1, &activeChannels_);

        // 
        // if (!workqueue.empty()) {

        // } else {
        //     poller_->poll(-1, &activeChannels_);
        // }

        for (ChannelList::iterator it = activeChannels_.begin();
             it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
            // 可以优化为异步执行

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
