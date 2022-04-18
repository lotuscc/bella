#pragma once

#include "ell_Channel.hpp"
#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"
#include <functional>

using ConnectionCallback = std::function<void(int fd, ell_Ipv4Addr *peerAddr)>;

using MessageCallback = ell_Channel::EventCallBack;
// using WriteCompleteCallback = ell_Channel::EventCallBack;

class ell_TcpServer {
private:
    ell_EventLoop *loop_;
    ell_TcpAcceptor *acceptor_;

    // ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    // WriteCompleteCallback writeCompleteCallback_;

public:
    ell_TcpServer();
    ~ell_TcpServer();

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    void loop();

    void defaultNewConnectionCallback(void);
    void defaultMessageCallback(int fd, ell_Ipv4Addr *peerAddr);

    // void setConnectionCallback(const ConnectionCallback &callback);
    // void setMessageCallback(const MessageCallback &callback);
    // void setWriteCompleteCallback(const WriteCompleteCallback &callback);

    void receive(int fd);
};

ell_TcpServer::ell_TcpServer() {
    loop_ = new ell_EventLoop();
    acceptor_ = new ell_TcpAcceptor();

    acceptor_->setConnectionCallback(
        std::bind(&ell_TcpServer::defaultMessageCallback, this,
                  std::placeholders::_1, std::placeholders::_2));
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::bind(ell_Ipv4Addr &localaddr) {
    acceptor_->bind(localaddr);
}

void ell_TcpServer::loop() { loop_->loop(); }

void ell_TcpServer::listen() {
    acceptor_->listen();

    auto accept = acceptor_->listenChannel();

    accept->enableReading();
    accept->set_readCallBack(
        std::bind(&ell_TcpServer::defaultNewConnectionCallback, this));

    // add listen event to epoll
    loop_->append_channel(accept);
}

void ell_TcpServer::receive(int fd) {
    char buf[512];
    memset(buf, '\0', sizeof buf);

    ell_Socket::recv_from(fd, buf, sizeof buf);

    LOG("receive: {} from {}", buf, fd);
}

void ell_TcpServer::defaultNewConnectionCallback(void) {

    auto newClient = acceptor_->accept();

    newClient->enableReading();
    newClient->set_readCallBack(messageCallback_);

    loop_->append_channel(newClient);
}

void ell_TcpServer::defaultMessageCallback(int fd, ell_Ipv4Addr *peerAddr) {
    messageCallback_ = std::bind(&ell_TcpServer::receive, this, fd);
}
