

#include "ell_EventLoop.hpp"
#include "ell_Ipv4Addr.hpp"
#include "ell_Message.hpp"
#include "ell_TcpConnector.hpp"

#include <cstring>
#include <unistd.h>

#include "ell_inputBuffer.hpp"
#include "ell_log.hpp"
#include "ell_message.pb.h"
#include "ell_outputBuffer.hpp"
#include "ell_ts_pool.hpp"

#include <chrono>
#include <thread>

using EventCallBack = ell_Channel::EventCallBack;

class ell_Shell {
private:
    ell_Channel *channel_;

    using ShellMessageCallBack = std::function<void(ell::ell_message)>;

    ShellMessageCallBack ShellMessageCallBack_;

public:
    ell_Shell();
    ~ell_Shell();

    void setShellMessageCallBack(ShellMessageCallBack callback) {
        ShellMessageCallBack_ = std::move(callback);
    }

    void handread();
    ell_Channel *channel();
};

ell_Shell::ell_Shell() {

    channel_ = new ell_Channel(nullptr, STDIN_FILENO);

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

    msg.set_magic(0x0717);
    msg.set_type(0x2);
    msg.set_content(buf, ret);

    if (ShellMessageCallBack_) {
        ShellMessageCallBack_(msg);
    }
}
