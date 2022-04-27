#pragma once

#include "ell_log.hpp"
#include "ell_socketOps.hpp"
#include <cstring>
#include <string>
#include <vector>

class ell_buffer {
    const static int kBUFFERSIZE = 1024;

    char input_data_[kBUFFERSIZE];
    char output_data_[kBUFFERSIZE];

    int iWriteIdx_ = 0;
    int iReadIdx_ = 0;

    int OWriteIdx_ = 0;
    int OreadIndex_ = 0;

public:
    ell_buffer();
    ~ell_buffer();

    ell_buffer(const ell_buffer &) = delete;
    ell_buffer &operator=(const ell_buffer &) = delete;

    // call by input buffer
    void send(int fd);
    void writeMessage(std::string msg);

    // call by output buffer
    void recv(int fd);
    std::string readMessage();
};

ell_buffer::ell_buffer() {
    memset(input_data_, '\0', kBUFFERSIZE);
    memset(output_data_, '\0', kBUFFERSIZE);
}

ell_buffer::~ell_buffer() {}

void ell_buffer::recv(int fd) {
    auto len = kBUFFERSIZE - iWriteIdx_;
    if (len == 0) {
        LOG("this is no free space!");
        return;
    }
    auto ret = sockops::recv(fd, &input_data_[iWriteIdx_], len, 0);
    iWriteIdx_ += ret;
}

void ell_buffer::send(int fd) {
    auto len = OWriteIdx_ - OreadIndex_;
    if (len == 0) {
        LOG("this is no data to send!");
        return;
    }
    auto ret = sockops::send(fd, &output_data_[OreadIndex_], len, 0);
    OreadIndex_ += ret;

    // 所有数据都发送完毕，缓冲归零
    if (OreadIndex_ == OWriteIdx_) {
        OreadIndex_ = 0;
        OWriteIdx_ = 0;
    }
}

void ell_buffer::writeMessage(std::string msg) {
    auto len = kBUFFERSIZE - iWriteIdx_;
    if (len < msg.size()) {
        LOG("this is no free space wtite message!");
        return;
    }
    for (auto &ch : msg) {
        output_data_[OWriteIdx_++] = ch;
    }
}

std::string ell_buffer::readMessage() {
    auto msg = std::string();
    while (iReadIdx_ < iWriteIdx_) {
        msg.push_back(input_data_[iReadIdx_++]);
    }

    // 所有数据都读取完毕，缓冲归零
    iReadIdx_ = 0;
    iWriteIdx_ = 0;
    return msg;
}