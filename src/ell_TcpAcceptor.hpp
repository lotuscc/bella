#pragma once

#include "ell_Channel.h"
#include "ell_Socket.hpp"

#include "ell_TcpConnector.hpp"
#include "ell_log.hpp"
#include <functional>

#include "ell_EventLoop.hpp"

// 只负责接受连接,并通过ConnectionCallback，将fd以及peerAddr回调给TcpServer
// 不负责Tcp连接的分配

// class ell_EventLoop;

class ell_TcpAcceptor {
private:
    ell_Socket *acceptSocket_;
    ell_Channel *acceptChannel_;

    ell_EventLoop* loop_;

    using ConnectionCallback =
        std::function<void(int fd, ell_Ipv4Addr *peerAddr)>;

    ConnectionCallback connectionCallback_;

    // 处理acceptChannel_上可读事件
    void handread();

public:
    ell_TcpAcceptor();
    ~ell_TcpAcceptor();

    ell_TcpAcceptor(const ell_TcpAcceptor &) = delete;
    ell_TcpAcceptor &operator=(const ell_TcpAcceptor &) = delete;

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    ell_Channel *listenChannel();

    // test
    int fd() { return acceptSocket_->fd(); }

    void setConnectionCallback(ConnectionCallback callback);
};

ell_TcpAcceptor::ell_TcpAcceptor() {
    acceptSocket_ = new ell_Socket();
    loop_ = new ell_EventLoop();
    acceptChannel_ = new ell_Channel(loop_, acceptSocket_->fd());
}

ell_TcpAcceptor::~ell_TcpAcceptor() {}

void ell_TcpAcceptor::bind(ell_Ipv4Addr &localaddr) {
    acceptSocket_->bind(localaddr);
}
void ell_TcpAcceptor::listen() {
    acceptSocket_->listen();

    acceptChannel_->enableReading();
    acceptChannel_->set_readCallBack(
        std::bind(&ell_TcpAcceptor::handread, this));
}

ell_Channel *ell_TcpAcceptor::listenChannel() { return acceptChannel_; }

void ell_TcpAcceptor::setConnectionCallback(ConnectionCallback callback) {
    connectionCallback_ = callback;
}

// 当此acceptSocket_ 上发生可读事件时
// 只有一种情况：有新连接到来
void ell_TcpAcceptor::handread() {
    auto client_addr = new ell_Ipv4Addr();
    int connfd = ::accept(acceptSocket_->fd(), client_addr->addr(),
                          client_addr->len_addr());
    assert(connfd >= 0);

    if (connectionCallback_) {
        connectionCallback_(connfd, client_addr);
    }
}