

#pragma once
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_socketOps.h"
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
