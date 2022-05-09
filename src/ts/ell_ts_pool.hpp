#pragma once

#include <atomic>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "ell_ts_queue.hpp"
#include "function_wrapper.hpp"

class ell_ts_pool {
    using work_t = function_wrapper;

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
    ell_ts_pool() : done(false) {
        // unsigned const thread_count = std::thread::hardware_concurrency() +
        // 3;

        unsigned const thread_count = 4;
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
    submit(FunctionType f) {
        using result_type = typename std::result_of<FunctionType()>::type;

        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());

        work_queue.push(std::move(task));

        return res;
    }
};
