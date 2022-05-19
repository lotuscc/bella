#pragma once

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_TcpAcceptor.h"
#include "ell_TcpConnector.h"
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
