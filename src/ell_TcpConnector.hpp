#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ell_Channel.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_buffer.hpp"
#include "ell_message.pb.h"

using ConnectionCallback = ell_Channel::EventCallBack;
using MessageCallback = ell_Channel::EventCallBack;
using WriteCompleteCallback = ell_Channel::EventCallBack;
using HighWaterMarkCallback = ell_Channel::EventCallBack;

class ell_TcpConnector {
private:
    ell_buffer buffer_;
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

    char buf[1024];

   auto ret = sockops::recv(socket_->fd(), buf, sizeof(buf), 0);

    // buffer_.recv(socket_->fd());
    // std::string msg = buffer_.readMessage();

    LOG("receive {} bytes \n", ret);

    ell::ell_message message;

    int32_t len = *(int32_t*)(&buf);

    LOG("message len: {} \n", len);

    auto pret = message.ParseFromArray(buf+4, ret-4);
    if(!pret){
        LOG("ParseFromArray failture! \n");
    }else{
        auto magic = message.magic();
        auto type = message.type();
        auto content = message.content();

        LOG("magic: {} type: {} content: {} \n", magic, type, content);
    }

    // LOG("receive {} bytes : {} from {} \n", ret, buf, socket_->fd());
}

ell_Channel *ell_TcpConnector::channel() { return channel_; }

void ell_TcpConnector::handread() { buffer_.recv(socket_->fd()); }
void ell_TcpConnector::handwrite() { buffer_.send(socket_->fd()); }
void ell_TcpConnector::handleClose() {}
void ell_TcpConnector::handleError() {}