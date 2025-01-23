// File: channel.h

#ifndef CHANNEL_H
#define CHANNEL_H
#include <deque>
#include <cstddef>

template<typename T>
class Channel {
public:
    explicit Channel(int capacity);

    bool channel_push(const T& element);
    bool channel_pop(T& element);
    bool channel_full() const;
    bool channel_empty() const;
    int channel_size() const;

private:
    std::deque<T> buffer;
    int capacity;
};

#include "channel.tpp"

#endif // CHANNEL_H