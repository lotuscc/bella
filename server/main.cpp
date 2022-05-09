
#include <functional>
#include <iostream>
#include <memory>

// #include "ell_Channel.h"
// #include "ell_EPoller.h"
// #include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.hpp"
#include "ell_TcpClient.hpp"
#include "ell_TcpServer.hpp"

void echo(ell::ell_message *message, ell::ell_message *ret) {
    // ret.set
    ret->set_magic(message->magic());
    ret->set_type(0x08);
    ret->set_content(message->content());
}

int main() {

    ell_Ipv4Addr local_addr;

    EventCallBack call;
    auto server = new ell_TcpServer(local_addr);

    // server->bind(local_addr);
    // server->listen();

    server->setdefaultMessage(echo);

    server->loop();

    return 0;
}