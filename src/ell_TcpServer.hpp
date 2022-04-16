#pragma once

#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"

class ell_TcpServer {
private:
    ell_EventLoop *loop_;
    ell_TcpAcceptor *acceptor_;

public:
    ell_TcpServer();
    ~ell_TcpServer();

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    void loop();


};

ell_TcpServer::ell_TcpServer() {
    loop_ = new ell_EventLoop();
    acceptor_ = new ell_TcpAcceptor();
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::bind(ell_Ipv4Addr &localaddr) {
    acceptor_->bind(localaddr);
}

void ell_TcpServer::loop() { loop_->loop(); }

void ell_TcpServer::listen() {
    acceptor_->listen();   

    // add listen event to epoll
    loop_->append_channel(acceptor_->listenChannel());

    loop_->poller_->set_listenfd(acceptor_->fd());
}