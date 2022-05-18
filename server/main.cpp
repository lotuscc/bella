
#include <functional>
#include <iostream>
#include <memory>

#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_TcpClient.h"
#include "ell_TcpServer.h"

using MessageCall = ell_TcpConnector::handerMessageCall;

void echo(ell::ell_message *message, ell::ell_message *ret, void *next) {
    ret->set_content(message->content());
}


void sayhello3(ell::ell_message *m, ell::ell_message *ret, void *next) {

    ret->set_content("hello3");

    *(MessageCall *)next = echo;
}

void sayhello2(ell::ell_message *m, ell::ell_message *ret, void *next) {

    ret->set_content("hello2");

    *(MessageCall *)next = sayhello3;
}


void sayhello(ell::ell_message *m, ell::ell_message *ret, void *next) {

    ret->set_content("hello1");

    *(MessageCall *)next = sayhello;
}

int main() {

    ell_Ipv4Addr local_addr;

    auto server = new ell_TcpServer(local_addr);

    server->setdefaultMessage(sayhello);

    server->loop();

    return 0;
}