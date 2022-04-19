#pragma once

#include "ell_Channel.hpp"
#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"
#include "ell_TcpConnector.hpp"
#include <functional>
#include <unordered_map>

class ell_TcpServer {
private:
    ell_EventLoop *loop_;
    ell_TcpAcceptor *acceptor_;
    ell_Ipv4Addr localAddr_;

    std::map<int, ell_TcpConnector *> Connectors_;

public:
    ell_TcpServer();
    ~ell_TcpServer();

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    void loop();

    void newConnection(int fd, ell_Ipv4Addr *peerAddr);
};

ell_TcpServer::ell_TcpServer() {
    loop_ = new ell_EventLoop();
    acceptor_ = new ell_TcpAcceptor();

    acceptor_->setConnectionCallback(std::bind(&ell_TcpServer::newConnection,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::bind(ell_Ipv4Addr &localaddr) {
    localAddr_ = localaddr;
    acceptor_->bind(localaddr);
}

void ell_TcpServer::loop() { loop_->loop(); }

void ell_TcpServer::listen() {
    acceptor_->listen();
    auto accept = acceptor_->listenChannel();

    // add listen event to epoll
    loop_->append_channel(accept);
}

void ell_TcpServer::newConnection(int fd, ell_Ipv4Addr *peerAddr) {
    // new client
    auto client = new ell_TcpConnector(fd, localAddr_, *peerAddr);

    Connectors_[fd] = client;

    // 分配TCP连接
    //
    loop_->append_channel(client->channel());
}