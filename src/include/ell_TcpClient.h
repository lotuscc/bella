

#include "ell_EventLoop.h"
#include "ell_Ipv4Addr.h"
#include "ell_TcpConnector.h"
#include "ell_log.h"
#include <cstring>
#include <unistd.h>

#include "ell_inputBuffer.h"
#include "ell_log.h"
#include "ell_message.pb.h"
#include "ell_outputBuffer.h"

#include "ell_shell.h"

#include <chrono>
#include <thread>

using EventCallBack = ell_Channel::EventCallBack;

class ell_TcpClient {
private:
    std::shared_ptr<ell_EventLoop> loop_;
    // ell_TcpConnector *connector_;
    ell_Socket socket_;
    ell_outputBuffer outbuffer_;
    ell_inputBuffer inbuffer_;

    ell_Channel channel_;
    ell_Shell shell_;

    // ts_queue<ell::ell_message> message_queue_;

    // ell_ts_pool ts_pool_;

public:
    ell_TcpClient();
    ~ell_TcpClient();

    void connect(ell_Ipv4Addr &serverAddr);
    void disconnect();
    void loop();

    EventCallBack NewConnectionCallBack_;
    EventCallBack MessageCallBack_;

    void sendMessage();
    void recvMessage();

    void handread();
    void handwrite();
    void handleClose();
    void handleError();

    void handleShellMessage(ell::ell_message msg);

    void sayhello();
};
