
#pragma once

#include <assert.h>
#include <boost/implicit_cast.hpp>
#include <map>
#include <memory>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <vector>

#include <iostream>

#include "ell_Channel.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"

using ChannelList = ell_Channel::ChannelList;
using EventCallBack = ell_Channel::EventCallBack;

class ell_EPoller {
    using EventList = std::vector<struct epoll_event>;
    using ChannelMap = std::map<int, ell_Channel *>;

    int epollfd_;
    EventList events_;
    ChannelMap listenChannels_;

    static const int kInitEventListSize = 16;

public:
    ell_EPoller(const ell_EPoller &) = delete;
    ell_EPoller &operator=(const ell_EPoller &) = delete;

    void poll(int timeoutMs, ChannelList *activeChannels);
    void fillActiveChannels(int numEvents, ChannelList *activeChannels);

    void append_listenChannel(ell_Channel *listenChannel);
    void remove_listenChannel(ell_Channel *listenChannel);

    ell_EPoller();
    ~ell_EPoller();
};

ell_EPoller::ell_EPoller() : epollfd_(::epoll_create1(EPOLL_CLOEXEC)) {
    events_ = std::vector<struct epoll_event>(kInitEventListSize);
}

ell_EPoller::~ell_EPoller() {}

void ell_EPoller::poll(int timeoutMs, ChannelList *activeChannels) {

    assert(events_.size() != 0);
    // 阻塞，获取事件
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
                                 static_cast<int>(events_.size()), timeoutMs);

    LOG("reveive events ! numEvent: {}", numEvents);

    //填充激活事件到activeChannels列表中
    fillActiveChannels(numEvents, activeChannels);
}

void ell_EPoller::fillActiveChannels(int numEvents,
                                     ChannelList *activeChannels) {
    for (int i = 0; i < numEvents; ++i) {
        auto newfd = events_[i].data.fd;
        // new connection
        if (listenChannels_.count(newfd)) {
            listenChannels_[newfd]->set_revents(events_[i].events);
            activeChannels->push_back(listenChannels_[newfd]);
            continue;
        } else {
            // error
        }
    }
}

void ell_EPoller::append_listenChannel(ell_Channel *listenChannel) {
    LOG("append");

    int fd = listenChannel->fd();

    struct epoll_event event;
    event.events = listenChannel->events();
    event.data.fd = fd;

    if (listenChannels_.count(fd)) {
        if (::epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event) < 0) {
            LOG("epoll ctl failure!");
        }
    } else {
        // insert
        listenChannels_[fd] = listenChannel;

        if (::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
            LOG("epoll ctl failure!");
        }
    }

    listenChannels_[fd] = listenChannel;
}
void ell_EPoller::remove_listenChannel(ell_Channel *listenChannel) {

    int fd = listenChannel->fd();

    struct epoll_event event;
    event.events = listenChannel->events();
    event.data.fd = fd;

    listenChannels_.erase(listenChannel->fd());

    if (::epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        LOG("epoll ctl failure!");
    }
}