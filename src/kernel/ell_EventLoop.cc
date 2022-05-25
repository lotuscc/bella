#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <utility>
#include <vector>

#include "ell_Channel.h"
#include "ell_EPoller.h"
#include "ell_EventLoop.h"
#include "ell_log.h"

void ell_EventLoop::loop() {
    while (true) {
        activeChannels_.clear();

        // 阻塞
        poller_->poll(-1, &activeChannels_);

        for (int i = 0; i < (int)activeChannels_.size(); ++i) {

            LOG("ell_EventLoop loop");
            // 同步执行
            activeChannels_[i]->handleEvent();
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
