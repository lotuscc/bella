#pragma once

#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_socketOps.h"
#include <cstring>
#include <string>
#include <vector>

#include "ell_outputBuffer.h"

void ell_outputBuffer::remake() {
    memset(input_data_, '\0', kBUFFERSIZE);
    oWriteIdx_ = 0;
    oReadIdx_ = 0;
}

ell_outputBuffer::ell_outputBuffer() : oWriteIdx_(0), oReadIdx_(0) {
    memset(input_data_, '\0', kBUFFERSIZE);
}

ell_outputBuffer::~ell_outputBuffer() {}

void ell_outputBuffer::send(int fd) {
    auto len = oWriteIdx_ - oReadIdx_;
    if (len == 0) {
        LOG("this is no data to send!");
        return;
    }
    auto ret = sockops::send(fd, &input_data_[oReadIdx_], len, 0);
    oReadIdx_ += ret;

    // 所有数据都发送完毕，缓冲归零
    if (oReadIdx_ == oWriteIdx_) {
        oReadIdx_ = 0;
        oWriteIdx_ = 0;
        LOG("idx reset! \n");
    }
}

void ell_outputBuffer::writeMessage(ell::ell_message &message) {
    auto space_len = kBUFFERSIZE - oWriteIdx_;

    int32_t len = message.ByteSizeLong();

    if (space_len < len) {
        LOG("this is no free space wtite message!");
        return;
    }
    ::strncpy(&input_data_[oWriteIdx_], (char *)&len, 4);

    message.SerializeToArray(&input_data_[oWriteIdx_ + 4], len);

    oWriteIdx_ += (len + 4);
}

void ell_outputBuffer::writeHttp(ell_httpResponse &response) {
    auto space_len = kBUFFERSIZE - oWriteIdx_;

    int len = response.SerializeToArray(&input_data_[oWriteIdx_], 0);

    oWriteIdx_ += len;
}