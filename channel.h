#ifndef CHANNEL_H
#define CHANNEL_H

#include <deque>
#include <iostream>

template<typename T>
class channel {
public:
    channel(size_t capacity) : capacity(capacity), is_full(false) {}

    void channel_push(const T& element) {
        if (buffer.size() >= capacity) {
            std::cout << "Buffer full..." << std::endl;
            return;
        }
        buffer.push_back(element);
        is_full = buffer.size() == capacity;
    }

    T channel_pop() {
        if (buffer.empty()) {
            std::cout << "Buffer empty..." << std::endl;
            return T();
        }
        T ret_val = buffer.front();
        buffer.pop_front();
        is_full = false;
        return ret_val;
    }

    T front() const {
        return buffer.front();
    }

    bool channel_full() const {
        return is_full;
    }

    bool channel_empty() const {
        return buffer.empty();
    }

    size_t channel_size() const {
        return buffer.size();
    }

private:
    std::deque<T> buffer;
    size_t capacity;
    bool is_full;
};

#endif
