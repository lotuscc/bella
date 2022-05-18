#pragma once

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_TcpAcceptor.h"
#include "ell_TcpConnector.h"
#include "ell_Works.h"
#include <bits/iterator_concepts.h>
#include <functional>
#include <memory>
#include <unordered_map>

#include "ell_conn_pool.h"

using MessageCall = ell_TcpConnector::handerMessageCall;

class ell_TcpServer {
private:
    ell_EventLoop *loop_;
    ell_TcpAcceptor *acceptor_;
    ell_Ipv4Addr localAddr_;

    std::map<int, ell_TcpConnector *> Connectors_;

    ell_Works works_;

    MessageCall messagecall_;

    ell_conn_pool* pool_;

public:
    ell_TcpServer(ell_Ipv4Addr& localAddr);
    ~ell_TcpServer();

    void loop();

    void defaultConnection(int fd, ell_Ipv4Addr *peerAddr);

    void setdefaultMessage(MessageCall call);
};
