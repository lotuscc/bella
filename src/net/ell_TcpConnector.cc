#pragma once

#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "ell_Channel.h"
#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_Socket.h"
#include "ell_inputBuffer.h"
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_outputBuffer.h"

#include "ell_TcpConnector.h"
#include "ell_socketOps.h"

#include "ell_httpRequest.h"
#include "ell_httpResponse.h"

using ConnectionCallback = ell_Channel::EventCallBack;
using MessageCallback = ell_Channel::EventCallBack;
using WriteCompleteCallback = ell_Channel::EventCallBack;
using HighWaterMarkCallback = ell_Channel::EventCallBack;

void ell_TcpConnector::remake(std::shared_ptr<ell_ts_pool> executor_pool,
                              std::shared_ptr<ell_EventLoop> loop, int fd,
                              ell_Ipv4Addr localAddr, ell_Ipv4Addr peerAddr) {
    localAddr_ = localAddr;
    peerAddr_ = peerAddr;
    executor_pool_ = executor_pool;

    socket_.remake(fd);
    channel_.remake(loop, fd);

    inbuffer_.remake();
    outbuffer_.remake();

    channel_.set_readCallBack(std::bind(&ell_TcpConnector::handread, this));
    channel_.set_writeCallBack(std::bind(&ell_TcpConnector::handwrite, this));
    channel_.set_closeCallBack(std::bind(&ell_TcpConnector::handleClose, this));
    channel_.set_errorCallBack(std::bind(&ell_TcpConnector::handleError, this));

    channel_.enableReading();
    channel_.enableClosing();
}

ell_TcpConnector::ell_TcpConnector(std::shared_ptr<ell_ts_pool> executor_pool,
                                   std::shared_ptr<ell_EventLoop> loop, int fd,
                                   ell_Ipv4Addr localAddr,
                                   ell_Ipv4Addr peerAddr)
    : localAddr_(localAddr), peerAddr_(peerAddr), socket_(fd),
      channel_(loop, fd), executor_pool_(executor_pool) {

    channel_.set_readCallBack(std::bind(&ell_TcpConnector::handread, this));
    channel_.set_writeCallBack(std::bind(&ell_TcpConnector::handwrite, this));
    channel_.set_closeCallBack(std::bind(&ell_TcpConnector::handleClose, this));
    channel_.set_errorCallBack(std::bind(&ell_TcpConnector::handleError, this));

    channel_.enableReading();
    channel_.enableClosing();
}

ell_TcpConnector::~ell_TcpConnector() {}

void ell_TcpConnector::defaultMessage(void) {}

// ell_Channel *ell_TcpConnector::channel() { return &channel_; }

void ell_TcpConnector::echo() {
    char buf[1024];
    memset(buf, '\0', 1024);
    sockops::recv(socket_.fd(), buf, sizeof(buf), 0);
    // sockops::send(socket_.fd(), buf, sizeof(buf), 0);

    std::cout << "content: \n" << buf << std::endl;
}

// ???????????????????????????
void ell_TcpConnector::work() {

    ell::ell_message recm;
    ell::ell_message retm;
    handerMessageCall next = nullptr;

    if (inbuffer_.tryReadMessage(recm)) {
        if (handerMessage) {
            handerMessage(&recm, &retm, (void *)&next);
        }
    }

    if (next != nullptr) {
        handerMessage = next;
    }
    int32_t len = retm.ByteSizeLong();
    LOG("len: {} \n", len);
    if (len > 0) {
        outbuffer_.writeMessage(retm);
        channel_.enableWriting();
    }
}

void ell_TcpConnector::httpWork() {
    ell_httpRequest req;
    ell_httpResponse resq;
    resq.setVersion("HTTP/1.1");
    resq.setStatus("200");
    resq.setReasonPhrase("Ok");
    resq.appendHeader("Content-Length", "13");
    resq.setBody("hello, world!");

    if (inbuffer_.tryReadHttp(req)) {
        outbuffer_.writeHttp(resq);
        channel_.enableWriting();
    }
}

void ell_TcpConnector::handread() {
    LOG("hand read! \n");
    // echo();
    // return;

    inbuffer_.recv(socket_.fd());

    // ????????????
    // auto f = std::bind(&ell_TcpConnector::work, this);
    auto f = std::bind(&ell_TcpConnector::httpWork, this);
    executor_pool_->submit(std::move(f));
}

void ell_TcpConnector::set_handerMessageCall(handerMessageCall call) {
    handerMessage = std::move(call);
}

// ???????????????????????????
void ell_TcpConnector::handwrite() {
    LOG("hand write! \n");
    outbuffer_.send(socket_.fd());
    channel_.disableWriting();
}
void ell_TcpConnector::handleClose() {
    LOG("hand close! \n");

    channel_.remove();
    sockops::shutdown(socket_.fd(), SHUT_RDWR);
}
void ell_TcpConnector::handleError() {
    LOG("hand error! \n");

    channel_.remove();
    sockops::shutdown(socket_.fd(), SHUT_RDWR);
    exit(1);
}