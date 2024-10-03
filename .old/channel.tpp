template<typename T>
Channel<T>::Channel(size_t capacity) : capacity(capacity) {}

template<typename T>
void Channel<T>::channel_push(const T& element) {
    if (buffer.size() >= capacity) {
        std::cout << "Channel is full!" << std::endl;
        return;
         // Channel is full
    }
    buffer.push_back(element);
    is_full = buffer.size() == capacity;
}

template<typename T>
T Channel<T>::channel_pop(T& element) {
    if (buffer.empty()) {
        return T(); // Channel is empty
    }
    element = buffer.front();
    buffer.pop_front();
    return element;
}

template<typename T>
bool Channel<T>::channel_full() const {
    return buffer.size() >= capacity;
}

template<typename T>
bool Channel<T>::channel_empty() const {
    return buffer.empty();
}

template<typename T>
size_t Channel<T>::channel_size() const {
    return buffer.size();
}
