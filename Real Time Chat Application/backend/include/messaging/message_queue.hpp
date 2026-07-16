#pragma once

#include "../common/types.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace chat {

class MessageQueue {
public:
    void              enqueue(Message message);
    std::optional<Message> dequeue();   // blocks until message available or stopped
    void              stop();

private:
    std::queue<Message>     queue_;
    std::mutex              mu_;
    std::condition_variable cv_;
    bool                    stopped_ = false;
};

} // namespace chat
