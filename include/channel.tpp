template<typename T>
Channel<T>::Channel(int capacity) : capacity(capacity) {}

template<typename T>
bool Channel<T>::channel_push(const T& element) {
    if (buffer.size() >= capacity) {
        return false; // Channel is full
    }
    buffer.push_back(element);
    return true;
}

template<typename T>
bool Channel<T>::channel_pop(T& element) {
    if (buffer.empty()) {
        return false; // Channel is empty
    }
    element = buffer.front();
    buffer.pop_front();
    return true;
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
int Channel<T>::channel_size() const {
    return buffer.size();
}
