#pragma once

#include "ell_Channel.hpp"
#include "ell_Socket.hpp"

#include "ell_log.hpp"
#include <functional>


class ell_TcpAcceptor {
private:
    ell_Socket *acceptSocket_;
    ell_Channel *acceptChannel_;

    using ConnectionCallback =
        std::function<void(int fd, ell_Ipv4Addr *peerAddr)>;

    ConnectionCallback connectionCallback_;

public:
    ell_TcpAcceptor();
    ~ell_TcpAcceptor();

    ell_TcpAcceptor(const ell_TcpAcceptor &) = delete;
    ell_TcpAcceptor &operator=(const ell_TcpAcceptor &) = delete;

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    ell_Channel *accept();
    ell_Channel *listenChannel();

    // test
    int fd() { return acceptSocket_->fd(); }

    void setConnectionCallback(ConnectionCallback callback);
};

ell_TcpAcceptor::ell_TcpAcceptor() {
    acceptSocket_ = new ell_Socket();
    acceptChannel_ = new ell_Channel(acceptSocket_->fd());
}

ell_TcpAcceptor::~ell_TcpAcceptor() {}

void ell_TcpAcceptor::bind(ell_Ipv4Addr &localaddr) {
    acceptSocket_->bind(localaddr);
}
void ell_TcpAcceptor::listen() { acceptSocket_->listen(); }

ell_Channel *ell_TcpAcceptor::accept() {

    auto client = new ell_Ipv4Addr();
    int connfd =
        ::accept(acceptSocket_->fd(), client->addr(), client->len_addr());
    assert(connfd >= 0);

    if (connectionCallback_) {
        connectionCallback_(connfd, client);
    }

    LOG("new connection connfd: {}", connfd);

    ell_Channel *newClient = new ell_Channel(connfd);

    return newClient;
}

ell_Channel *ell_TcpAcceptor::listenChannel() {    
    return acceptChannel_;
}

void ell_TcpAcceptor::setConnectionCallback(ConnectionCallback callback) {
    connectionCallback_ = callback;
}
