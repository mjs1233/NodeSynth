#pragma once
#include <cinttypes>
#include <array>
#include <optional>
#include <exception>
#include <stdexcept>

template <typename T, size_t L>
class CircularQueue {
private:

    size_t front_idx;
    size_t rear_idx;

    std::array<T, L + 1> data;


public:
    CircularQueue() : front_idx(0), rear_idx(0) {

    }



    constexpr size_t max_size() {
        return L;
    }

    size_t size() {

        if (front_idx > rear_idx) {
            return L + 1 - front_idx + rear_idx;
        }

        return rear_idx - front_idx;
    }

    void push(T value) {

        if ((rear_idx + 1) % (L + 1) == front_idx)
            throw std::out_of_range("Queue is full");
        
        data[rear_idx] = value;
        rear_idx = (rear_idx + 1) % (L + 1);
    }

    T& front() {

        if(front_idx == rear_idx)
            throw std::out_of_range("Queue is empty");

        return data[front_idx];
    }

    void pop() {

        if (front_idx == rear_idx)
            throw std::out_of_range("Queue is empty");

        front_idx = (front_idx + 1) % (L + 1);
    }
};