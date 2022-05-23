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

#include "ell_Works.h"

ell_Works::ell_Works() {}
ell_Works::~ell_Works() {}

void ell_Works::append(ell_TcpConnector *conn) {
    // loop_->appendChannel(conn->channel());

    // Connectors_[conn->channel()->fd()] = conn;
}