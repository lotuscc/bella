

#pragma once
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_socketOps.h"
#include <cstring>
#include <string>
#include <vector>

#include "ell_inputBuffer.h"

#include "ell_httpRequestParser.h"

void ell_inputBuffer::remake() {
    memset(output_data_, '\0', kBUFFERSIZE);
    iWriteIdx_ = 0;
    iReadIdx_ = 0;
}

ell_inputBuffer::ell_inputBuffer() : iWriteIdx_(0), iReadIdx_(0) {
    memset(output_data_, '\0', kBUFFERSIZE);
}

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

        auto content = message.content();

        LOG("content: {} \n", content);
    }

    return true;
}


bool ell_inputBuffer::tryReadHttp() {

    const char *request =
        "POST /index.html HTTP/1.1\r\nconnection:close\r\ncontent-length: "
        "1\r\n\r\n1\r\n\r\n";
    int request_len = strlen(request);

    ell_httpRequestParser ellparser;
    auto x = ellparser.execute(request, request_len);    

    return true;
}

bool ell_inputBuffer::istry(void) { return (iWriteIdx_ - iReadIdx_) >= 40; }