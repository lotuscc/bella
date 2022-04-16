#pragma once

#include "ell_Channel.hpp"
#include "ell_Socket.hpp"

class ell_TcpAcceptor {
private:
    ell_Socket *acceptSocket_;
    ell_Channel *acceptChannel_;

public:
    ell_TcpAcceptor();
    ~ell_TcpAcceptor();

    ell_TcpAcceptor(const ell_TcpAcceptor &) = delete;
    ell_TcpAcceptor &operator=(const ell_TcpAcceptor &) = delete;

    void bind(ell_Ipv4Addr &localaddr);
    void listen();
    static ell_Channel *accept(int fd);

    ell_Channel *listenChannel();

    // test
    int fd() { return acceptSocket_->fd(); }
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

ell_Channel *ell_TcpAcceptor::accept(int fd) {

    auto client = new ell_Ipv4Addr();
    int connfd = ::accept(fd, client->addr(), client->len_addr());
    assert(connfd >= 0);

    std::cout << "accept new connection connfd: " << connfd << std::endl;

    ell_Channel *newClient = new ell_Channel(connfd);

    newClient->enableReading();

    return newClient;
}

ell_Channel *ell_TcpAcceptor::listenChannel() {
    acceptChannel_->enableReading();

    // acceptChannel_->set_readCallBack(std::bind(&ell_TcpAcceptor::accept,
    // this));

    return acceptChannel_;
}