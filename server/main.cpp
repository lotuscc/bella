
#include <functional>
#include <iostream>
#include <memory>

#include "ell_Channel.hpp"
#include "ell_EPoller.hpp"
#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Socket.hpp"
#include "ell_TcpClient.hpp"
#include "ell_TcpServer.hpp"

int main() {

    ell_Ipv4Addr local_addr;

    auto server = new ell_TcpServer();
    server->bind(local_addr);
    server->listen();

    server->loop();

    return 0;
}