#pragma once

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_TcpAcceptor.h"
#include "ell_TcpConnector.h"
#include "ell_Works.h"
#include <bits/iterator_concepts.h>
#include <functional>
#include <memory>
#include <unordered_map>

#include "ell_conn_pool.h"

#include "ell_TcpServer.h"

ell_TcpServer::ell_TcpServer(ell_Ipv4Addr &localAddr)
    : localAddr_(localAddr), pool_(2),
      loop_(std::make_shared<ell_EventLoop>(nullptr)),
      acceptor_(loop_, localAddr_) {

    acceptor_.setConnectionCallback(std::bind(&ell_TcpServer::defaultConnection,
                                              this, std::placeholders::_1,
                                              std::placeholders::_2));
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::loop() {
    // 开始监听是否有连接到来
    loop_->loop();
}

void ell_TcpServer::defaultConnection(int fd, ell_Ipv4Addr *peerAddr) {
    // 分配TCP连接

    // auto client =
    //     new ell_TcpConnector(pool_.getLoop(), fd, localAddr_, *peerAddr);
    // client->set_handerMessageCall(messagecall_);

    if (Connectors_.contains(fd)) {
        Connectors_[fd]->remake(pool_.getLoop(), fd, localAddr_, *peerAddr);
    } else {
        Connectors_[fd] = std::make_shared<ell_TcpConnector>(
            pool_.getLoop(), fd, localAddr_, *peerAddr);
    }

    Connectors_[fd]->set_handerMessageCall(messagecall_);
}

void ell_TcpServer::setdefaultMessage(MessageCall call) {
    messagecall_ = std::move(call);
}