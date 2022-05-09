#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ell_Channel.h"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_inputBuffer.hpp"
#include "ell_log.hpp"
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
    using handerMessageCall =
        std::function<void(ell::ell_message *message, ell::ell_message *ret)>;

    ell_TcpConnector(int fd, ell_Ipv4Addr localAddr, ell_Ipv4Addr peerAddr);
    ~ell_TcpConnector();

    ell_TcpConnector(const ell_TcpConnector &) = delete;
    ell_TcpConnector &operator=(const ell_TcpConnector &) = delete;

    void handread();
    void handwrite();
    void handleClose();
    void handleError();

    void defaultMessage(void);

    handerMessageCall handerMessage;
    void set_handerMessageCall(handerMessageCall call);

    ell_Channel *channel();
};

ell_TcpConnector::ell_TcpConnector(int fd, ell_Ipv4Addr localAddr,
                                   ell_Ipv4Addr peerAddr)
    : localAddr_(localAddr), peerAddr_(peerAddr) {

    socket_ = new ell_Socket(fd);
    channel_ = new ell_Channel(nullptr, fd);
    channel_->enableReading();
    channel_->set_readCallBack(std::bind(&ell_TcpConnector::handread, this));

    // channel_->enableWriting();
    channel_->set_writeCallBack(std::bind(&ell_TcpConnector::handwrite, this));

    channel_->enableClosing();
    channel_->set_closeCallBack(
        std::bind(&ell_TcpConnector::handleClose, this));

    channel_->set_errorCallBack(
        std::bind(&ell_TcpConnector::handleError, this));
}

ell_TcpConnector::~ell_TcpConnector() {}

void ell_TcpConnector::defaultMessage(void) {}

ell_Channel *ell_TcpConnector::channel() { return channel_; }

void ell_TcpConnector::handread() {
    inbuffer_.recv(socket_->fd());
    ell::ell_message message;
    if (inbuffer_.tryReadMessage(message)) {
        // outbuffer_.writeMessage(message);
        // outbuffer_.send(socket_->fd());
        ell::ell_message retm;

        if (handerMessage) {
            handerMessage(&message, &retm);
            int32_t len = retm.ByteSizeLong();

            LOG("len: {} \n", len);
            // if (len > 0) {
            outbuffer_.writeMessage(message);
            outbuffer_.send(socket_->fd());
            // }
        }
    }
}

void ell_TcpConnector::set_handerMessageCall(handerMessageCall call) {
    handerMessage = call;
}

void ell_TcpConnector::handwrite() {
    LOG("hand write! \n");
    outbuffer_.send(socket_->fd());
    // channel_->disableWriting();
}
void ell_TcpConnector::handleClose() {
    LOG("hand close! \n");

    // loop_->remove_Channel(channel_);
    sockops::shutdown(socket_->fd(), SHUT_RDWR);

    exit(1);
}
void ell_TcpConnector::handleError() {
    LOG("hand error! \n");
    // loop_->remove_Channel(channel_);
    exit(1);
}