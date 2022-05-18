#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_inputBuffer.h"
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_outputBuffer.h"

#include "ell_TcpConnector.h"

using ConnectionCallback = ell_Channel::EventCallBack;
using MessageCallback = ell_Channel::EventCallBack;
using WriteCompleteCallback = ell_Channel::EventCallBack;
using HighWaterMarkCallback = ell_Channel::EventCallBack;


ell_TcpConnector::ell_TcpConnector(ell_EventLoop *loop, int fd,
                                   ell_Ipv4Addr localAddr,
                                   ell_Ipv4Addr peerAddr)
    : localAddr_(localAddr), peerAddr_(peerAddr) {

    socket_ = new ell_Socket(fd);
    channel_ = new ell_Channel(loop, fd);

    channel_->set_readCallBack(std::bind(&ell_TcpConnector::handread, this));
    channel_->set_writeCallBack(std::bind(&ell_TcpConnector::handwrite, this));
    channel_->set_closeCallBack(
        std::bind(&ell_TcpConnector::handleClose, this));
    channel_->set_errorCallBack(
        std::bind(&ell_TcpConnector::handleError, this));

    channel_->enableReading();
    channel_->enableClosing();
}

ell_TcpConnector::~ell_TcpConnector() {}

void ell_TcpConnector::defaultMessage(void) {}

ell_Channel *ell_TcpConnector::channel() { return channel_; }

void ell_TcpConnector::handread() {
    inbuffer_.recv(socket_->fd());

    ell::ell_message message;

    if (inbuffer_.tryReadMessage(message)) {
        ell::ell_message retm;

        handerMessageCall nextmessagecall = nullptr;

        if (handerMessage) {
            handerMessage(&message, &retm, (void *)&nextmessagecall);

            if (nextmessagecall != nullptr) {
                handerMessage = nextmessagecall;
            }
            int32_t len = retm.ByteSizeLong();
            LOG("len: {} \n", len);
            if (len > 0) {
                outbuffer_.writeMessage(retm);
                channel_->enableWriting();
            }
        }
    }
}

void ell_TcpConnector::set_handerMessageCall(handerMessageCall call) {
    handerMessage = std::move(call);
}

void ell_TcpConnector::handwrite() {
    LOG("hand write! \n");
    outbuffer_.send(socket_->fd());
    channel_->disableWriting();
}
void ell_TcpConnector::handleClose() {
    LOG("hand close! \n");

    channel_->remove();
    sockops::shutdown(socket_->fd(), SHUT_RDWR);
}
void ell_TcpConnector::handleError() {
    LOG("hand error! \n");

    channel_->remove();
    sockops::shutdown(socket_->fd(), SHUT_RDWR);
    exit(1);
}