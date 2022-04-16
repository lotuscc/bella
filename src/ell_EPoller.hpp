
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
#include "ell_TcpAcceptor.hpp"
#include "ell_Socket.hpp"

using ChannelList = ell_Channel::ChannelList;
using EventCallBack = ell_Channel::EventCallBack;

class ell_EPoller {
    // using epoll_data_t = union epoll_data {
    //     void *ptr;
    //     int fd;
    //     uint32_t u32;
    //     uint64_t u64;
    // };

    // struct epoll_event {
    //     uint32_t events;
    //     epoll_data_t data;
    // };
    using EventList = std::vector<struct epoll_event>;
    using ChannelMap = std::map<int, ell_Channel *>;

    int epollfd_;
    EventList events_;
    ChannelMap channels_;

    static const int kInitEventListSize = 16;

    int listenfd_;

public:
    ell_EPoller(const ell_EPoller &) = delete;
    ell_EPoller &operator=(const ell_EPoller &) = delete;

    void poll(int timeoutMs, ChannelList *activeChannels);
    void fillActiveChannels(int numEvents, ChannelList *activeChannels);

    void update(int operation, ell_Channel *channel);

    void set_listenfd(int listenfd) { listenfd_ = listenfd; }

    void receive(int fd);

    ell_EPoller();
    ~ell_EPoller();
};

ell_EPoller::ell_EPoller() : epollfd_(::epoll_create1(EPOLL_CLOEXEC)) {
    events_ = std::vector<struct epoll_event>(kInitEventListSize);
}

ell_EPoller::~ell_EPoller() {}

void ell_EPoller::poll(int timeoutMs, ChannelList *activeChannels) {

    assert(events_.size() != 0);
    // 阻塞
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
                                 static_cast<int>(events_.size()), timeoutMs);

    std::cout << "reveive events ! numEvent: " << numEvents << std::endl;
    //
    fillActiveChannels(numEvents, activeChannels);
}

void ell_EPoller::fillActiveChannels(int numEvents,
                                     ChannelList *activeChannels) {
    for (int i = 0; i < numEvents; ++i) {

        if (events_[i].data.fd == listenfd_) {
            // channel->set_readCallBack();
            // new connection
            // channel->set_readCallBack();

            // std::cout << "new connection" << std::endl;

            // activeChannels->push_back(channel);
            auto client = ell_TcpAcceptor::accept(listenfd_);

            update(0, client);

            // activeChannels->push_back(client);

            continue;
        }

        // 填入fd
        ell_Channel *channel = new ell_Channel(events_[i].data.fd);

        // 填入revents
        channel->set_revents(events_[i].events);

        EventCallBack call = std::bind(&ell_EPoller::receive, this, channel->fd());

        channel->set_readCallBack(call);

        activeChannels->push_back(channel);
    }
}

void ell_EPoller::receive(int fd){
    char buf[512];
    memset(buf, '\0', sizeof buf);

    ell_Socket::recv_from(fd, buf, sizeof buf);

    std::cout << "receive:" << buf << "  from: " << fd <<std::endl;
}


// void ell_EPoller::set_listen_event(int listenfd) {
//     this->listenfd_ = listenfd;

//     struct epoll_event event;
//     event.events = EPOLLIN | EPOLLET;
//     event.data.fd = listenfd;

//     if (::epoll_ctl(epollfd_, EPOLL_CTL_ADD, listenfd, &event) < 0) {
//     }
// }

void ell_EPoller::update(int operation, ell_Channel *channel) {
    struct epoll_event event;

    int fd = channel->fd();
    // event.events = channel->events();
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;

    if (::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        std::cout << "epoll ctl failure! " << std::endl;
    }
}

// 注册 EPOLLIN 事件和 EPOLLET 事件
// EPOLLIN： 数据（包括普通数据和优先数据）可读
// EPOLLET： 采用 ET 模式，可读、可写只发送一次事件通知
// static void addfd(int epollfd, int fd){
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = EPOLLIN | EPOLLET;
//     epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

//     set_nonblocking(fd);
// }