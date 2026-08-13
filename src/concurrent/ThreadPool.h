#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace editor {

// Fixed-size worker pool. submit() returns a future; the underlying task
// stays alive via the queued std::function even if the caller discards the
// future (fire-and-forget submissions are the common case here).
//
// On destruction, workers finish draining the queue before joining (a
// graceful shutdown, not an abrupt cancel) - see the worker loop below.
class ThreadPool {
public:
    explicit ThreadPool(size_t threads = std::max(2u, std::thread::hardware_concurrency()));
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <typename F>
    auto submit(F&& f) -> std::future<decltype(f())> {
        using R = decltype(f());
        auto task = std::make_shared<std::packaged_task<R()>>(std::forward<F>(f));
        std::future<R> fut = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        return fut;
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
};

} // namespace editor
