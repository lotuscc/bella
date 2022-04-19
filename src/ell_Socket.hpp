#pragma once

#include <cstddef>
#include <iostream>
#include <sys/socket.h>

#include "ell_Ipv4Addr.hpp"
#include "ell_log.hpp"

class ell_Socket {
private:
    const int sockfd_;

public:
    ell_Socket();
    ell_Socket(int fd);
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

    inline static void recv_from(int fd, void *__buf, size_t __n);
    inline static void send_to(int fd, const void *__buf, size_t __n);
    inline static void connection_to(int __fd, const struct sockaddr *__addr,
                                     socklen_t __len);
};

ell_Socket::ell_Socket() : sockfd_(::socket(PF_INET, SOCK_STREAM, 0)) {}

ell_Socket::ell_Socket(int fd) : sockfd_(fd) {}

ell_Socket::~ell_Socket() {}

void ell_Socket::connection(ell_Ipv4Addr &serverAddr) {
    auto x = ::connect(sockfd_, serverAddr.addr(), serverAddr.len());
    LOG("connect reveive: {}", x);
    if (x < 0) {
        LOG("connection failure ");
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
        LOG("send error!");
    }
}

void ell_Socket::recv(void *__buf, size_t __n) {
    ::recv(sockfd_, __buf, __n, 0);
}

void ell_Socket::send_to(int fd, const void *buf, size_t n) {
    ::send(fd, buf, n, 0);
}

void ell_Socket::recv_from(int fd, void *__buf, size_t __n) {
    ::recv(fd, __buf, __n, 0);
}

void ell_Socket::connection_to(int __fd, const struct sockaddr *__addr,
                               socklen_t __len) {
    ::connect(__fd, __addr, __len);
}