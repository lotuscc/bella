#pragma once

#include "ell_Channel.h"
#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpAcceptor.hpp"
#include "ell_TcpConnector.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

class ell_Works {
private:
    ell_EventLoop *loop_;
    std::map<int, ell_TcpConnector *> Connectors_;

    // void handler func(ResponseWriter, *Request)
public:
    ell_Works();
    ~ell_Works();

    ell_Works(const ell_Works &) = delete;
    ell_Works &operator=(const ell_Works &) = delete;

    void append(ell_TcpConnector *conn);
};

ell_Works::ell_Works() {}
ell_Works::~ell_Works() {}

void ell_Works::append(ell_TcpConnector *conn) {
    loop_->append_channel(conn->channel());

    Connectors_[conn->channel()->fd()] = conn;
}