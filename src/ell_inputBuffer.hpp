

#pragma once
#include "ell_log.hpp"
#include "ell_message.pb.h"
#include "ell_socketOps.hpp"
#include <cstring>
#include <string>
#include <vector>

class ell_inputBuffer {
    const static int kBUFFERSIZE = 1024;

    char output_data_[kBUFFERSIZE];

    int iWriteIdx_ = 0;
    int iReadIdx_ = 0;

public:
    ell_inputBuffer();
    ~ell_inputBuffer();

    ell_inputBuffer(const ell_inputBuffer &) = delete;
    ell_inputBuffer &operator=(const ell_inputBuffer &) = delete;

    void recv(int fd);
    std::string readMessage();
    bool tryReadMessage(ell::ell_message &message);

    bool istry(void);
};

ell_inputBuffer::ell_inputBuffer() { memset(output_data_, '\0', kBUFFERSIZE); }

ell_inputBuffer::~ell_inputBuffer() {}

void ell_inputBuffer::recv(int fd) {
    auto len = kBUFFERSIZE - iWriteIdx_;
    if (len == 0) {
        LOG("this is no free space!");

        // 将数据移动到Buffer前面

        return;
    }
    auto ret = sockops::recv(fd, &output_data_[iWriteIdx_], len, 0);
    iWriteIdx_ += ret;
}

bool ell_inputBuffer::tryReadMessage(ell::ell_message &message) {
    int data_len = iWriteIdx_ - iReadIdx_;

    if (data_len < 4) {
        return false;
    }
    // assert();
    int32_t len = *(int32_t *)(&output_data_[iReadIdx_]);

    if (data_len < len + 4) {
        return false;
    }

    LOG("message len: {} \n", len);

    auto pret = message.ParseFromArray(&output_data_[iReadIdx_ + 4], len);

    iReadIdx_ += (len + 4);
    if (iReadIdx_ == iWriteIdx_) {
        iReadIdx_ = 0;
        iWriteIdx_ = 0;
        LOG("idx reset! \n");
    }

    if (!pret) {
        LOG("ParseFromArray failture! \n");
    } else {
        auto magic = message.magic();
        auto type = message.type();
        auto content = message.content();

        LOG("magic: {} type: {} content: {} \n", magic, type, content);
    }

    return true;
}

bool ell_inputBuffer::istry(void) {
    return (iWriteIdx_ - iReadIdx_) >= 40;
}