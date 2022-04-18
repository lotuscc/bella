#pragma once

#include "ell_buffer.hpp"
class ell_TcpConnector {
private:
    ell_buffer* readBuffer;

public:
    ell_TcpConnector();
    ~ell_TcpConnector();

    ell_TcpConnector(const ell_TcpConnector &) = delete;
    ell_TcpConnector &operator=(const ell_TcpConnector &) = delete;

    void handread();
    void handwrite();
    void handleClose();
    void handleError();
};

ell_TcpConnector::ell_TcpConnector() {
    readBuffer = new ell_buffer();
}

ell_TcpConnector::~ell_TcpConnector() {}
