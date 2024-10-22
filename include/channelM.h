#ifndef CHANNELM_H
#define CHANNELM_H

#include <deque>
#include <stdexcept>
#include <cstddef> // For size_t

/**
 * @brief A channel class implementing a FIFO buffer with capacity constraints using std::deque.
 *
 * @tparam T The type of elements stored in the channel.
 */
template<typename T>
class channelM {
public:
    static const size_t DEFAULT_CAPACITY = 128; // Add a default capacity

    // Constructor with default capacity
    channelM(size_t capacity = DEFAULT_CAPACITY) : capacity(capacity) {}

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
    size_t size() const {
        return buffer.size();
    }

    // Get the capacity of the channel
    size_t get_capacity() const {
        return capacity;
    }

private:
    std::deque<T> buffer;     // The underlying buffer
    size_t capacity;          // Maximum capacity of the channel

    // Disable copying
    channelM(const channelM&) = delete;
    channelM& operator=(const channelM&) = delete;
};

#endif // CHANNELM_H
