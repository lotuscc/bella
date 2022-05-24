#pragma once

#include "ell_Channel.h"
#include "ell_Socket.h"

#include "ell_TcpConnector.h"
#include "ell_log.h"
#include <functional>

#include "ell_EventLoop.h"

#include "ell_TcpAcceptor.h"

ell_TcpAcceptor::ell_TcpAcceptor(std::shared_ptr<ell_EventLoop> loop, ell_Ipv4Addr &localAddr)
    : acceptSocket_(), acceptChannel_(loop, acceptSocket_.fd()){
    // acceptSocket_ = new ell_Socket();

    // 开启监听
    acceptSocket_.bind(localAddr);
    acceptSocket_.listen();

    // acceptChannel_ = new ell_Channel(loop, acceptSocket_->fd());
    acceptChannel_.enableReading();
    acceptChannel_.set_readCallBack(
        std::bind(&ell_TcpAcceptor::handread, this));
}

ell_TcpAcceptor::~ell_TcpAcceptor() {}

ell_Channel *ell_TcpAcceptor::listenChannel() { return &acceptChannel_; }

void ell_TcpAcceptor::setConnectionCallback(ConnectionCallback callback) {
    connectionCallback_ = callback;
}

// 当此acceptSocket_ 上发生可读事件时
// 只有一种情况：有新连接到来
void ell_TcpAcceptor::handread() {
    auto client_addr = new ell_Ipv4Addr();
    int connfd = ::accept(acceptSocket_.fd(), client_addr->addr(),
                          client_addr->len_addr());
    assert(connfd >= 0);

    if (connectionCallback_) {
        connectionCallback_(connfd, client_addr);
    }
}