

#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_TcpConnector.h"

#include <cstring>
#include <string>
#include <unistd.h>

#include "ell_inputBuffer.h"
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_outputBuffer.h"

#include "ell_EventLoop.h"

#include <chrono>
#include <thread>

#include "ell_shell.h"

void ell_Shell::setShellMessageCallBack(ShellMessageCallBack callback) {
    ShellMessageCallBack_ = std::move(callback);
}

ell_Shell::ell_Shell(ell_EventLoop *loop) {

    channel_ = new ell_Channel(loop, STDIN_FILENO);

    channel_->enableReading();
    channel_->set_readCallBack(std::bind(&ell_Shell::handread, this));
}

ell_Shell::~ell_Shell() {}

ell_Channel *ell_Shell::channel() { return channel_; }

void ell_Shell::handread() {

    char buf[1024];

    memset(buf, '\0', 1024);

    auto ret = read(STDIN_FILENO, buf, sizeof(buf));

    // auto ret = sockops::recv(STDIN_FILENO, buf, sizeof(buf), 0);

    LOG("shell {} bytes:  {} \n", ret, buf);

    ell::ell_message msg;
    msg.set_content(buf, ret);

    if (ShellMessageCallBack_) {
        ShellMessageCallBack_(msg);
    }
}
