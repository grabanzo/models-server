#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>


namespace grabanzo::mserver {

template <typename T>
class ThreadQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(std::move(value));
        cv_.notify_one();
    }

    // Retorna std::optional para manejar el caso de que la cola sea cerrada.
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !queue_.empty() || is_shutdown_; });

        if (is_shutdown_ && queue_.empty()) {
            return std::nullopt;
        }

        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            is_shutdown_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    bool is_shutdown_ = false;
};

} // namespace grabanzo::mserver::core