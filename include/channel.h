// File: include/channel.h

#ifndef CHANNEL_H
#define CHANNEL_H

#include <deque>
#include <cstddef>
#include <mutex>
#include <condition_variable>

template<typename T>
class Channel {
public:
    explicit Channel(size_t capacity);

    bool channel_push(const T& element);

    bool channel_pop(T& element);

    bool channel_full() const;

    bool channel_empty() const;

    size_t channel_size() const;

private:
    std::deque<T> buffer;              // Buffer to store elements
    size_t capacity;                   // Maximum capacity
    mutable std::mutex mtx_;           // Mutex for thread safety
    std::condition_variable cv_full_;  // Condition variable for full channel
    std::condition_variable cv_empty_; // Condition variable for empty channel
};

#endif // CHANNEL_H
