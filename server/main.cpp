
#include <functional>
#include <iostream>
#include <memory>

#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_TcpClient.h"
#include "ell_TcpServer.h"

using MessageCall = ell_TcpConnector::handerMessageCall;

void sayhello2(ell::ell_message *m, ell::ell_message *ret, void *next) {
    ret->set_content("hello2");
}

void sayhello(ell::ell_message *m, ell::ell_message *ret, void *next) {
    ret->set_content("hello");
    *(MessageCall *)next = sayhello2;
}

int main() {

    ell_Ipv4Addr local_addr;

    auto server = new ell_TcpServer(local_addr);

    server->setdefaultMessage(sayhello);

    server->loop();

    return 0;
}