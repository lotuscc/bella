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
public:
    using handerMessageCall = std::function<void(
        ell::ell_message *message, ell::ell_message *ret, void *)>;

private:
    ell_outputBuffer outbuffer_;
    ell_inputBuffer inbuffer_;

    ell_Ipv4Addr localAddr_;
    ell_Ipv4Addr peerAddr_;

    ell_Socket socket_;
    ell_Channel channel_;

    handerMessageCall handerMessage;

    // ConnectionCallback connectionCallback_;
    // MessageCallback messageCallback_;
    // WriteCompleteCallback writeCompleteCallback_;
    // HighWaterMarkCallback highWaterMarkCallback_;
public:

public:
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

    void echo();

    void set_handerMessageCall(handerMessageCall call);

    // ell_Channel *channel();

    void make_clean();
    void remake(ell_EventLoop *loop, int fd, ell_Ipv4Addr localAddr,
                ell_Ipv4Addr peerAddr);
};
