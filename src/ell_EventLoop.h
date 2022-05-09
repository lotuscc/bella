#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <vector>


class ell_Channel;
class ell_EPoller;

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
