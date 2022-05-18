
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

#include "ell_Channel.h"


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

    void updateChannel(ell_Channel *listenChannel);

    ell_EPoller();
    ~ell_EPoller();
};
