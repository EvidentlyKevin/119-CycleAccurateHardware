#ifndef CHANNEL_H
#define CHANNEL_H

#include <deque>

template<typename T>
class Channel {
public:
    Channel(size_t capacity);

    void channel_push(T& element);
    T channel_pop();
    bool channel_full();
    bool channel_empty();
    size_t channel_size();

private:
    std::deque<T> buffer;
    size_t capacity;
    bool is_full;
};

#include "channel.tpp" 

#endif // CHANNEL_H
