// File: include/channel.tpp

#ifndef CHANNEL_TPP
#define CHANNEL_TPP

#include "channel.h"

// Constructor
template<typename T>
Channel<T>::Channel(size_t capacity) : capacity(capacity) {}

// Push an element into the channel. Returns false if full.
template<typename T>
bool Channel<T>::channel_push(const T& element) {
    std::unique_lock<std::mutex> lock(mtx_);
    if (buffer.size() >= capacity) {
        return false; // Channel is full
    }
    buffer.push_back(element);
    cv_empty_.notify_one();
    return true;
}

// Pop an element from the channel. Returns false if empty.
template<typename T>
bool Channel<T>::channel_pop(T& element) {
    std::unique_lock<std::mutex> lock(mtx_);
    if (buffer.empty()) {
        return false; // Channel is empty
    }
    element = buffer.front();
    buffer.pop_front();
    cv_full_.notify_one();
    return true;
}

// Check if the channel is full.
template<typename T>
bool Channel<T>::channel_full() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return buffer.size() >= capacity;
}

// Check if the channel is empty.
template<typename T>
bool Channel<T>::channel_empty() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return buffer.empty();
}

// Get the current size of the channel.
template<typename T>
size_t Channel<T>::channel_size() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return buffer.size();
}

#endif // CHANNEL_TPP
