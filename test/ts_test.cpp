#include <algorithm>
#include <atomic>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "ell_ts_pool.hpp"

#include "ell_message.pb.h"

#include "ell_inputBuffer.hpp"
#include "ell_outputBuffer.hpp"

void ts_main() {
    ell_ts_pool pool;

    auto f = [](int i) { std::cout << i << ": hello, world!" << std::endl; };

    for (int i = 0; i < 8; ++i) {
        auto res = pool.submit(std::bind(f, i));
    }
}

void protobuf_main() {
    ell::ell_message message;
    message.set_magic(0x0717);
    message.set_type(1);
    message.set_content("hello, world!");
}

int main() {
    protobuf_main();
    return 0;
}
