#include <atomic>
#include <future>
#include <vector>

#include "concurrent/EventQueue.h"
#include "concurrent/ThreadPool.h"
#include "harness.h"

using namespace editor;

TEST(threadpool_runs_all_submitted_tasks) {
    ThreadPool pool(4);
    std::atomic<int> counter{0};
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 200; ++i) {
        futures.push_back(pool.submit([&counter] {
            counter++;
            return 0;
        }));
    }
    for (auto& f : futures) f.get();
    CHECK_EQ(counter.load(), 200);
}

TEST(event_queue_is_thread_safe_under_concurrent_push) {
    EventQueue queue;
    ThreadPool pool(4);
    const int perThread = 100;
    std::vector<std::future<int>> futures;
    for (int t = 0; t < 4; ++t) {
        futures.push_back(pool.submit([&queue, perThread] {
            for (int i = 0; i < perThread; ++i) {
                queue.push(DictionaryLoadedEvent{static_cast<size_t>(i)});
            }
            return 0;
        }));
    }
    for (auto& f : futures) f.get();
    auto drained = queue.drainAll();
    CHECK_EQ(drained.size(), static_cast<size_t>(4 * perThread));
}

TEST(drain_all_empties_the_queue) {
    EventQueue queue;
    queue.push(DictionaryLoadedEvent{1});
    queue.push(DictionaryLoadedEvent{2});
    CHECK_EQ(queue.drainAll().size(), static_cast<size_t>(2));
    CHECK_EQ(queue.drainAll().size(), static_cast<size_t>(0));
}
