#ifndef PORT_H
#define PORT_H

#include <iostream>
#include <vector>

template<typename T>
class Port{
    private:
        std::vector<T> data;
    
    public:

         // Constructor
    Port() = default;

    // Move constructor
    Port(Port&& other) noexcept : data(std::move(other.data)) {
        // other.data is now in a valid but unspecified state
    }

    // Move assignment operator
    Port& operator=(Port&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            // other.data is now in a valid but unspecified state
        }
        return *this;
    }
        // Enqueue a single value
        void enqueue(int value) {
            data.push_back(value);
        }
    
        // Enqueue multiple values
        void enqueueMultiple(const std::vector<int>& values) {
            data.insert(data.end(), values.begin(), values.end());
        }
    
        // Dequeue all elements and return them
        std::vector<int> dequeueAll() {
            std::vector<int> poppedData = data; // Store current elements
            data.clear(); // Clear the queue
            return poppedData; // Return removed elements
        }

       // Dequeue single element and return it
    int dequeue() {
        if (data.empty()) {
            throw std::out_of_range("Queue is empty!");
        }
        int poppedValue = data.front(); // Store the front value
        data.erase(data.begin());       // Remove the first element
        return poppedValue;             // Return the removed value
    }
    
        // Get front element
        int front() {
            if (!data.empty()) {
                return data.front();
            }
            throw std::out_of_range("Queue is empty!");
        }
    
        // Check if queue is empty
        bool isEmpty() {
            return data.empty();
        }
    
        // Get size of queue
        int size() {
            return data.size();
        }
    };

    #endif // VECTORQUEUE_H