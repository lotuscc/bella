#include <algorithm>
#include <atomic>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "ell_ts_pool.hpp"

#include "ell_message.pb.h"

#include "ell_inputBuffer.h"
#include "ell_outputBuffer.h"

#include "ell_conn_pool.h"

#include "function_wrapper.hpp"

void ts_main() {
    ell_ts_pool pool;

    auto f = [](int i) { std::cout << i << ": hello, world!" << std::endl; };

    for (int i = 0; i < 8; ++i) {
        auto res = pool.submit(std::bind(f, i));
    }
}

void protobuf_main() {
    ell::ell_message message;
    message.set_content("hello, world!");
}

void conn_pool_test() {
    auto pool = new ell_conn_pool();
    // pool->getLoop(0)
}

void function_wrapper_test() {
    std::cout << "function_wrapper_test" << std::endl;
}

void function(void *, void *) {}

int main() {

    function_wrapper f;
    std::function<void(void)> fx = function_wrapper_test;
    
    f = std::move(fx);

    f();
    // fx();
    // function_wrapper_test();

    // f();

    return 0;
}
