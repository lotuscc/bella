#pragma once

#include "ell_ts_pool.h"
#pragma once

#include <atomic>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "ell_EventLoop.h"
#include "ell_ts_pool.h"
#include <assert.h>

#include <memory>

class ell_conn_pool {
    using Executor = std::function<void(void)>;

    std::atomic_bool done;
    std::vector<std::shared_ptr<ell_EventLoop>> pool;
    int nth = 0;

    std::shared_ptr<ell_ts_pool> executor_pool;

public:
    ell_conn_pool(int nthreads) : done(false) {
        // unsigned const thread_count = std::thread::hardware_concurrency();

        executor_pool = std::make_shared<ell_ts_pool>(1);    

        try {
            for (int i = 0; i < nthreads; ++i) {
                auto eventloop = std::make_shared<ell_EventLoop>(executor_pool);
                auto t = std::jthread(&ell_EventLoop::loop, eventloop);
                t.detach();
                pool.push_back(eventloop);
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    std::shared_ptr<ell_EventLoop> getLoop() {
        nth %= pool.size();
        if (nth < 0 || nth >= (int)pool.size()) {
            return nullptr;
        }
        return pool[nth++];
    }

    ~ell_conn_pool() { done = true; }
};
