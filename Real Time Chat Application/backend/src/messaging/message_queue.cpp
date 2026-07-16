#include "messaging/message_queue.hpp"

namespace chat {

void MessageQueue::enqueue(Message message) {
    {
        std::lock_guard<std::mutex> lock(mu_);
        queue_.push(std::move(message));
    }
    cv_.notify_one();
}

std::optional<Message> MessageQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mu_);
    cv_.wait(lock, [this] { return !queue_.empty() || stopped_; });
    if (stopped_ && queue_.empty()) return std::nullopt;
    Message msg = std::move(queue_.front());
    queue_.pop();
    return msg;
}

void MessageQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(mu_);
        stopped_ = true;
    }
    cv_.notify_all();
}

} // namespace chat
