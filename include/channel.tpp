#include <iostream>

template<typename T>
Channel<T>::Channel(size_t capacity) : capacity(capacity), is_full(false) {}

template<typename T>
void Channel<T>::channel_push(T& element) {
    if (buffer.size() >= capacity) {
        std::cout << "Buffer full..." << std::endl;
        return;
    }
    buffer.push_back(element);
    is_full = buffer.size() == capacity;
}

template<typename T>
T Channel<T>::channel_pop() {
    if (buffer.empty()) {
        return T();
    }
    T ret_val = buffer.front();
    buffer.pop_front();
    is_full = false;
    return ret_val;
}

template<typename T>
bool Channel<T>::channel_full() {
    return is_full;
}

template<typename T>
bool Channel<T>::channel_empty() {
    return buffer.empty();
}

template<typename T>
size_t Channel<T>::channel_size() {
    return buffer.size();
}
