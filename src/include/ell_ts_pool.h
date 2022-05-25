#pragma once

#include <atomic>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "ell_ts_queue.h"
#include "function_wrapper.hpp"

class ell_ts_pool {
    // using work_t = function_wrapper;
    using work_t = std::function<void()>;

    std::atomic_bool done;
    ts_queue<work_t> work_queue;
    std::vector<std::jthread> threads;

    void worker_thread() {

        while (!done) {
            work_t task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    ell_ts_pool(unsigned thread_count) : done(false) {
        // unsigned const thread_count = 4;
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::jthread(&ell_ts_pool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~ell_ts_pool() { done = true; }

    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    get_res(FunctionType f) {
        using result_type = typename std::result_of<FunctionType()>::type;

        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());

        work_queue.push(std::move(task));

        return res;
    }

    template <typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void()>(f));
    }
};
