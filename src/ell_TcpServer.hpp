#pragma once

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"
#include "ell_TcpConnector.hpp"
#include "ell_Works.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

using handerMessageCall = ell_TcpConnector::handerMessageCall;

class ell_TcpServer {
private:
    ell_EventLoop *loop_;
    ell_TcpAcceptor *acceptor_;
    ell_Ipv4Addr localAddr_;

    std::map<int, ell_TcpConnector *> Connectors_;

    ell_Works works_;

    handerMessageCall messagecall_;

    void appendhandler();
    // void handler func(ResponseWriter, *Request)();

public:
    ell_TcpServer(ell_Ipv4Addr& localAddr);
    ~ell_TcpServer();

    void loop();

    void newConnection(int fd, ell_Ipv4Addr *peerAddr);

    void setdefaultMessage(handerMessageCall call);
};
ell_TcpServer::ell_TcpServer(ell_Ipv4Addr& localAddr) {
    loop_ = new ell_EventLoop();
    acceptor_ = new ell_TcpAcceptor(loop_, localAddr);

    acceptor_->setConnectionCallback(std::bind(&ell_TcpServer::newConnection,
                                               this, std::placeholders::_1,
                                               std::placeholders::_2));
}

ell_TcpServer::~ell_TcpServer() {}

void ell_TcpServer::loop() { loop_->loop(); }

void ell_TcpServer::newConnection(int fd, ell_Ipv4Addr *peerAddr) {
    // new client
    auto client = new ell_TcpConnector(fd, localAddr_, *peerAddr);

    client->set_handerMessageCall(messagecall_);

    // works_.append(client);

    Connectors_[fd] = client;
    // 分配TCP连接
    //
    loop_->append_channel(client->channel());
}

void ell_TcpServer::setdefaultMessage(handerMessageCall call) {
    messagecall_ = call;
}