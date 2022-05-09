
#include <functional>
#include <iostream>
#include <memory>

// #include "ell_Channel.h"
// #include "ell_EPoller.hpp"
// #include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.h"
// #include "ell_Socket.hpp"
#include "ell_TcpClient.hpp"
// #include "ell_TcpServer.hpp"

int main() {
    ell_Ipv4Addr server_addr;

    auto client = new ell_TcpClient();

    client->connect(server_addr);

    client->loop();

    return 0;
}
