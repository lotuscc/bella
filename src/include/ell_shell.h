#pragma once


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

using EventCallBack = ell_Channel::EventCallBack;

class ell_Shell {
private:
    ell_Channel *channel_;

    using ShellMessageCallBack = std::function<void(ell::ell_message)>;

    ShellMessageCallBack ShellMessageCallBack_;

public:
    ell_Shell(std::shared_ptr<ell_EventLoop> loop);
    ~ell_Shell();

    void setShellMessageCallBack(ShellMessageCallBack callback);
    void handread();
    ell_Channel *channel();

    void appendCmd(std::string);
};
