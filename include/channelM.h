// File: include/channelM.h

#ifndef CHANNELM_H
#define CHANNELM_H

#include <deque>
#include <stdexcept>
#include <cstddef> // For int

template<typename T>
class channelM {
public:
    static const int DEFAULT_CAPACITY = 128; // Make this easily changeable

    // Constructor with default capacity
    channelM(int capacity = DEFAULT_CAPACITY) : capacity(capacity) {}

    // Move constructor
    channelM(channelM<T>&& other) noexcept
        : buffer(std::move(other.buffer)), capacity(other.capacity) {
        other.capacity = 0;
    }

    // Move assignment operator
    channelM<T>& operator=(channelM<T>&& other) noexcept {
        if (this != &other) {
            buffer = std::move(other.buffer);
            capacity = other.capacity;
            other.capacity = 0;
        }
        return *this;
    }

    // Push an element into the channel
    bool push(const T& element) {
        if (buffer.size() >= capacity) {
            // Buffer is full
            return false; // Indicate failure
        }
        buffer.push_back(element);
        return true; // Indicate success
    }

    // Pop an element from the channel
    bool pop(T& out_element) {
        if (buffer.empty()) {
            // Buffer is empty
            return false; // Indicate failure
        }
        out_element = buffer.front();
        buffer.pop_front();
        return true; // Indicate success
    }

    // Peek at the front element
    T front() const {
        if (buffer.empty()) {
            throw std::underflow_error("Channel buffer is empty");
        }
        return buffer.front();
    }

    // Check if the channel is full
    bool is_full() const {
        return buffer.size() >= capacity;
    }

    // Check if the channel is empty
    bool is_empty() const {
        return buffer.empty();
    }

    // Get the current size of the channel
    int size() const {
        return buffer.size();
    }

    // Get the capacity of the channel
    int get_capacity() const {
        return capacity;
    }

private:
    std::deque<T> buffer;     // The underlying buffer
    int capacity;          // Maximum capacity of the channel

    // Disable copying
    channelM(const channelM&) = delete;
    channelM& operator=(const channelM&) = delete;
};

#endif // CHANNELM_H
