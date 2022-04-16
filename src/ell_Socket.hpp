#pragma once

#include <iostream>

#include "ell_Ipv4Addr.hpp"


class ell_Socket {
private:
    const int sockfd_;

public:
    ell_Socket();
    ~ell_Socket();

    ell_Socket(const ell_Socket &) = delete;
    ell_Socket &operator=(const ell_Socket &) = delete;

    // call by Channel ...
    int fd() const;

    // call by TcpServer
    void bind(ell_Ipv4Addr &localaddr);

    // call by TcpServer
    void listen();

    // call by TcpClient
    void connection(ell_Ipv4Addr &serverAddr);

    void send(const void *__buf, size_t __n);
    void recv(void *__buf, size_t __n);

    static void recv_from(int fd, void *__buf, size_t __n);

};

ell_Socket::ell_Socket() : sockfd_(::socket(PF_INET, SOCK_STREAM, 0)) {}

ell_Socket::~ell_Socket() {}

void ell_Socket::connection(ell_Ipv4Addr &serverAddr) {
    auto x = ::connect(sockfd_, serverAddr.addr(), serverAddr.len());
    std::cout << "connect reveive: " << x << std::endl;
    if (x < 0) {
        std::cout << "connection failure " << std::endl;
    }
}

void ell_Socket::bind(ell_Ipv4Addr &localaddr) {
    ::bind(sockfd_, localaddr.addr(), localaddr.len());
}

void ell_Socket::listen() { ::listen(sockfd_, 5); }

int ell_Socket::fd() const { return sockfd_; }

void ell_Socket::send(const void *__buf, size_t __n) {
    int ret = ::send(sockfd_, __buf, __n, 0);
    if (ret == -1) {
        std::cout << "send error!" << std::endl;
    }
}

void ell_Socket::recv_from(int fd, void *__buf, size_t __n) {
    ::recv(fd, __buf, __n, 0);
}

void ell_Socket::recv(void *__buf, size_t __n) {
    ::recv(sockfd_, __buf, __n, 0);
}