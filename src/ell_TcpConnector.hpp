#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ell_Channel.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_inputBuffer.hpp"
#include "ell_message.pb.h"
#include "ell_outputBuffer.hpp"

using ConnectionCallback = ell_Channel::EventCallBack;
using MessageCallback = ell_Channel::EventCallBack;
using WriteCompleteCallback = ell_Channel::EventCallBack;
using HighWaterMarkCallback = ell_Channel::EventCallBack;

class ell_TcpConnector {
private:
    ell_outputBuffer outbuffer_;
    ell_inputBuffer inbuffer_;
    ell_Socket *socket_;
    ell_Channel *channel_;

    const ell_Ipv4Addr localAddr_;
    const ell_Ipv4Addr peerAddr_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;

public:
    ell_TcpConnector(int fd, ell_Ipv4Addr localAddr, ell_Ipv4Addr peerAddr);
    ~ell_TcpConnector();

    ell_TcpConnector(const ell_TcpConnector &) = delete;
    ell_TcpConnector &operator=(const ell_TcpConnector &) = delete;

    void handread();
    void handwrite();
    void handleClose();
    void handleError();

    void defaultMessage(void);

    ell_Channel *channel();
};

ell_TcpConnector::ell_TcpConnector(int fd, ell_Ipv4Addr localAddr,
                                   ell_Ipv4Addr peerAddr)
    : localAddr_(localAddr), peerAddr_(peerAddr) {

    socket_ = new ell_Socket(fd);
    channel_ = new ell_Channel(fd);
    channel_->set_readCallBack(
        std::bind(&ell_TcpConnector::defaultMessage, this));
}

ell_TcpConnector::~ell_TcpConnector() {}

void ell_TcpConnector::defaultMessage(void) {

    inbuffer_.recv(socket_->fd());

    ell::ell_message message;
    if (inbuffer_.tryReadMessage(message)) {
    }
}

ell_Channel *ell_TcpConnector::channel() { return channel_; }

void ell_TcpConnector::handread() {}
void ell_TcpConnector::handwrite() {}
void ell_TcpConnector::handleClose() {}
void ell_TcpConnector::handleError() {}