#pragma once

#pragma once

#include <atomic>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "ell_EventLoop.h"

#include <assert.h>

class ell_conn_pool {
    using Executor = std::function<void(void)>;

    std::atomic_bool done;
    std::vector<ell_EventLoop *> pool;

public:
    ell_conn_pool() : done(false) {
        // unsigned const thread_count = std::thread::hardware_concurrency();

        unsigned const thread_count = 4;
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                auto eventloop = new ell_EventLoop();

                auto t = std::jthread(&ell_EventLoop::loop, eventloop);
                t.detach();
                pool.push_back(eventloop);
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ell_EventLoop *getLoop(int i) {
        if (i < 0 || i >= pool.size()) {
            return nullptr;
        }

        return pool[i];
    }

    ~ell_conn_pool() { done = true; }
};
