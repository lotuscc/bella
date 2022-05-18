
#include <functional>
#include <iostream>
#include <memory>

#include "ell_Ipv4Addr.h"
#include "ell_TcpClient.h"

int main() {
    ell_Ipv4Addr server_addr;

    auto client = new ell_TcpClient();

    client->connect(server_addr);

    client->loop();

    return 0;
}
