

#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Message.hpp"
#include "ell_TcpConnector.hpp"
#include "ell_log.hpp"
#include <cstring>
#include <unistd.h>

#include "ell_inputBuffer.hpp"
#include "ell_log.hpp"
#include "ell_message.pb.h"
#include "ell_outputBuffer.hpp"
#include "ell_ts_pool.hpp"
#include "ell_ts_queue.hpp"

#include "ell_shell.hpp"

#include <chrono>
#include <thread>

using EventCallBack = ell_Channel::EventCallBack;

class ell_TcpClient {
private:
    ell_EventLoop *loop_;
    ell_TcpConnector *connector_;
    ell_Socket *socket_;
    ell_outputBuffer outbuffer_;
    ell_inputBuffer inbuffer_;

    ell_Channel *channel_;

    ell_Shell shell_;

    ts_queue<ell::ell_message> message_queue_;

    // ell_ts_pool ts_pool_;

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

    void handread();
    void handwrite();
    void handleClose();
    void handleError();

    void handleShellMessage(ell::ell_message msg);

    void sayhello();
};

ell_TcpClient::ell_TcpClient() {
    socket_ = new ell_Socket();
    loop_ = new ell_EventLoop();

    channel_ = new ell_Channel(loop_, socket_->fd());

    channel_->enableReading();
    channel_->set_readCallBack(std::bind(&ell_TcpClient::handread, this));

    // channel_->enableWriting();
    channel_->set_writeCallBack(std::bind(&ell_TcpClient::handwrite, this));

    channel_->enableClosing();
    channel_->set_closeCallBack(std::bind(&ell_TcpClient::handleClose, this));

    channel_->set_errorCallBack(std::bind(&ell_TcpClient::handleError, this));

    shell_.setShellMessageCallBack(std::bind(&ell_TcpClient::handleShellMessage,
                                             this, std::placeholders::_1));
}

ell_TcpClient::~ell_TcpClient() {}

void ell_TcpClient::connect(ell_Ipv4Addr &serverAddr) {
    socket_->connection(serverAddr);
}
void ell_TcpClient::disconnect() {}

void ell_TcpClient::loop() {

    loop_->append_channel(channel_);
    loop_->append_channel(shell_.channel());

    // std::async(std::bind(&ell_TcpClient::sayhello, this));

    // std::thread t(std::bind(&ell_TcpClient::sayhello, this));
    // t.detach();

    LOG("start  loop! \n");
    loop_->loop();
}

void ell_TcpClient::sayhello() {
    ell::ell_message message;
    message.set_magic(0x0717);
    message.set_type(1);
    message.set_content("hello, world!");

    outbuffer_.writeMessage(message);
    outbuffer_.send(socket_->fd());

    while (true) {
        std::chrono::milliseconds ms(2000);
        std::this_thread::sleep_for(ms);

        outbuffer_.writeMessage(message);
        // outbuffer_.send(socket_->fd());
        channel_->enableWriting();
        loop_->update_Channel(channel_);
    }
}

void ell_TcpClient::handread() {
    inbuffer_.recv(socket_->fd());
    ell::ell_message message;
    if (inbuffer_.tryReadMessage(message)) {
        // outbuffer_.writeMessage(message);
    }
}
void ell_TcpClient::handwrite() {
    LOG("hand write! \n");
    outbuffer_.send(socket_->fd());
    
    // 关闭可写
    channel_->disableWriting();
    loop_->update_Channel(channel_);
}
void ell_TcpClient::handleClose() {
    LOG("hand close! \n");

    loop_->remove_Channel(channel_);
    sockops::shutdown(socket_->fd(), SHUT_RDWR);

    exit(1);
}
void ell_TcpClient::handleError() {
    LOG("hand error! \n");
    loop_->remove_Channel(channel_);
    exit(1);
}

void ell_TcpClient::handleShellMessage(ell::ell_message msg) {
    if (msg.type() == 0x2) {
        outbuffer_.writeMessage(msg);
        outbuffer_.send(socket_->fd());
    }
}