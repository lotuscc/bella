#pragma once

#include <cstring>
#include <vector>

class ell_buffer {
    const static int kBUFFERSIZE = 1024;

    char data_[kBUFFERSIZE];

    int writeIndex_ = 0;
    int readIndex_ = 0;

public:
    ell_buffer();
    ~ell_buffer();

    ell_buffer(const ell_buffer &) = delete;
    ell_buffer &operator=(const ell_buffer &) = delete;

    void append();

    void read();
    void write();
};

ell_buffer::ell_buffer() { memset(data_, '\0', kBUFFERSIZE); }

ell_buffer::~ell_buffer() {}

void ell_buffer::read() {
    auto len = kBUFFERSIZE - writeIndex_;
    // sockops::read();
}
void ell_buffer::write() {}