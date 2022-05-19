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
    using handerMessageCall = std::function<void(
        ell::ell_message *message, ell::ell_message *ret, void *)>;

    ell_TcpConnector(ell_EventLoop *loop, int fd, ell_Ipv4Addr localAddr,
                     ell_Ipv4Addr peerAddr);
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
