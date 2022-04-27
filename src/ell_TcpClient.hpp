

#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Message.hpp"
#include "ell_TcpConnector.hpp"
#include "ell_log.hpp"
#include <unistd.h>

#include "ell_message.pb.h"

using EventCallBack = ell_Channel::EventCallBack;

class ell_TcpClient {
private:
    ell_EventLoop *loop_;
    ell_TcpConnector *connector_;
    ell_Socket *socket_;

public:
    ell_TcpClient();
    ~ell_TcpClient();

    void connect(ell_Ipv4Addr &serverAddr);
    void disconnect();
    void loop();

    EventCallBack NewConnectionCallBack_;
    EventCallBack MessageCallBack_;

    void sendMessage();
    void recvMessage();

    void sayHello() {
        char buf[512];
        memset(buf, '\0', sizeof buf);
        strcpy(buf, "hello, world!");

        LOG("send hello to server ");

        socket_->send(buf, sizeof buf);
    }
};

ell_TcpClient::ell_TcpClient() {
    socket_ = new ell_Socket();
    loop_ = new ell_EventLoop();
}

ell_TcpClient::~ell_TcpClient() {}

void ell_TcpClient::connect(ell_Ipv4Addr &serverAddr) {
    socket_->connection(serverAddr);
}
void ell_TcpClient::disconnect() {}

void ell_TcpClient::loop() {
    // auto client_w = new ell_Channel(socket_->fd());

    // client_w->enableWriting();
    // client_w->set_writeCallBack(std::bind(&ell_TcpClient::sayHello, this));

    // loop_->append_channel(client_w);
    ell::ell_message message;
    message.set_magic(0x0717);
    message.set_type(1);
    message.set_content("hello, world!");

    char buf[1024];
    memset(buf, '\0', sizeof buf);

    auto len = message.ByteSizeLong();
    message.SerializeToArray(buf, len);

    while (true) {
        // strcpy(buf, "hello, world!");

        LOG("send {} bytes to server ", len);

        socket_->send(buf, len);

        sleep(2);
    }

    // loop_->loop();
}