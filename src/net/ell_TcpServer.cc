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

ell_TcpServer::ell_TcpServer(ell_Ipv4Addr& localAddr) {
    pool_ = new ell_conn_pool();    
    loop_ = new ell_EventLoop();
    acceptor_ = new ell_TcpAcceptor(loop_, localAddr);

    acceptor_->setConnectionCallback(std::bind(&ell_TcpServer::defaultConnection,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::loop() { loop_->loop(); }


void ell_TcpServer::defaultConnection(int fd, ell_Ipv4Addr *peerAddr) {
    // new client
    // auto client = new ell_TcpConnector(loop_, fd, localAddr_, *peerAddr);

    auto client = new ell_TcpConnector(pool_->getLoop(0), fd, localAddr_, *peerAddr);


    client->set_handerMessageCall(messagecall_);

    // works_.append(client);

    Connectors_[fd] = client;
    // 分配TCP连接
    //
    // loop_->append_channel(client->channel());
}

void ell_TcpServer::setdefaultMessage(MessageCall call) {
    messagecall_ = std::move(call);
}