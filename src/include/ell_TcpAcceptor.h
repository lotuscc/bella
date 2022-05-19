#pragma once

#include "ell_Channel.h"
#include "ell_Socket.h"

#include "ell_TcpConnector.h"
#include "ell_log.h"
#include <functional>

#include "ell_EventLoop.h"

// 只负责接受连接,并通过ConnectionCallback，将fd以及peerAddr回调给TcpServer
// 不负责Tcp连接的分配
class ell_TcpAcceptor {
private:
    ell_Socket *acceptSocket_;
    ell_Channel *acceptChannel_;
    ell_EventLoop *loop_;

    using ConnectionCallback =
        std::function<void(int fd, ell_Ipv4Addr *peerAddr)>;

    ConnectionCallback connectionCallback_;

    // 处理acceptChannel_上可读事件
    void handread();

public:
    ell_TcpAcceptor(ell_EventLoop *loop, ell_Ipv4Addr &localAddr);
    ~ell_TcpAcceptor();

    ell_TcpAcceptor(const ell_TcpAcceptor &) = delete;
    ell_TcpAcceptor &operator=(const ell_TcpAcceptor &) = delete;

    ell_Channel *listenChannel();

    // test
    int fd() { return acceptSocket_->fd(); }

    void setConnectionCallback(ConnectionCallback callback);
};