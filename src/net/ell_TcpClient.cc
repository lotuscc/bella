

#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_TcpConnector.h"
#include "ell_log.h"
#include <cstring>
#include <unistd.h>

#include "ell_inputBuffer.h"
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_outputBuffer.h"

#include "ell_shell.h"

#include <chrono>
#include <thread>


#include "ell_TcpClient.h"

ell_TcpClient::ell_TcpClient() {
    socket_ = new ell_Socket();
    loop_ = new ell_EventLoop();

    channel_ = new ell_Channel(loop_, socket_->fd());
    channel_->set_readCallBack(std::bind(&ell_TcpClient::handread, this));
    channel_->set_closeCallBack(std::bind(&ell_TcpClient::handleClose, this));
    channel_->set_errorCallBack(std::bind(&ell_TcpClient::handleError, this));
    channel_->set_writeCallBack(std::bind(&ell_TcpClient::handwrite, this));

    channel_->enableReading();
    channel_->enableClosing();

    shell_ = new ell_Shell(loop_);
    shell_->setShellMessageCallBack(std::bind(
        &ell_TcpClient::handleShellMessage, this, std::placeholders::_1));
}

ell_TcpClient::~ell_TcpClient() {}

void ell_TcpClient::connect(ell_Ipv4Addr &serverAddr) {
    socket_->connection(serverAddr);
}
void ell_TcpClient::disconnect() {}

void ell_TcpClient::loop() {
    LOG("start  loop! \n");
    loop_->loop();
}

void ell_TcpClient::sayhello() {
    ell::ell_message message;
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
    }
}
void ell_TcpClient::handwrite() {
    LOG("hand write! \n");
    outbuffer_.send(socket_->fd());

    // 关闭可写
    channel_->disableWriting();
}
void ell_TcpClient::handleClose() {
    LOG("hand close! \n");

    channel_->remove();
    sockops::shutdown(socket_->fd(), SHUT_RDWR);

    exit(1);
}
void ell_TcpClient::handleError() {
    LOG("hand error! \n");
    channel_->remove();
    exit(1);
}

void ell_TcpClient::handleShellMessage(ell::ell_message msg) {
    outbuffer_.writeMessage(msg);
    channel_->enableWriting();
}