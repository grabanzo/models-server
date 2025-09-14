#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>


namespace grabanzo::mserver::core {

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one();
    }

    // Retorna std::optional para manejar el caso de que la cola sea cerrada.
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty() || is_shutdown; });

        if (is_shutdown && queue.empty()) {
            return std::nullopt;
        }

        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            is_shutdown = true;
        }
        cv.notify_all();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    bool is_shutdown = false;
};

} // namespace grabanzo::mserver::core