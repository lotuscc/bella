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
    int nth = 0;

public:
    ell_conn_pool(int nthreads) : done(false) {
        // unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            for (int i = 0; i < nthreads; ++i) {
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

    ell_EventLoop *getLoop() {
        nth %= pool.size();
        if (nth < 0 || nth >= (int)pool.size()) {
            return nullptr;
        }
        return pool[nth++];
    }

    ~ell_conn_pool() { done = true; }
};
