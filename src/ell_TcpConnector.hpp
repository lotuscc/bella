#pragma once

#include <functional>
#include <memory>

#include "ell_Channel.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_buffer.hpp"

using ConnectionCallback = ell_Channel::EventCallBack;
using MessageCallback = ell_Channel::EventCallBack;
using WriteCompleteCallback = ell_Channel::EventCallBack;
using HighWaterMarkCallback = ell_Channel::EventCallBack;

class ell_TcpConnector {
private:
    ell_buffer *readBuffer;

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

    void receive(int fd);

    ell_Channel *channel();
};

ell_TcpConnector::ell_TcpConnector(int fd, ell_Ipv4Addr localAddr,
                                   ell_Ipv4Addr peerAddr)
    : localAddr_(localAddr), peerAddr_(peerAddr) {

    socket_ = new ell_Socket(fd);
    channel_ = new ell_Channel(fd);

    readBuffer = new ell_buffer();
}

ell_TcpConnector::~ell_TcpConnector() {}

void ell_TcpConnector::receive(int fd) {
    char buf[512];
    memset(buf, '\0', sizeof buf);

    ell_Socket::recv_from(fd, buf, sizeof buf);

    LOG("receive: {} from {}", buf, fd);
}

ell_Channel *ell_TcpConnector::channel() {

    channel_->set_readCallBack(
        std::bind(&ell_TcpConnector::receive, this, socket_->fd()));

    return channel_;
}

void ell_TcpConnector::handread() { receive(socket_->fd()); }
void ell_TcpConnector::handwrite() {}
void ell_TcpConnector::handleClose() {}
void ell_TcpConnector::handleError() {}