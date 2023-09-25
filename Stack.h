#ifndef UNTITLED13_STACK_H
#define UNTITLED13_STACK_H
#include <iostream>
#include "Allocator.h"

template <typename T>
class Stack {
private:
    Allocator<T> allocator;
    T* arr;
    unsigned int capacity;
    unsigned int size;

    void resize(unsigned int newCapacity) {
        T* newArr = allocator.allocate(capacity);
        for (unsigned int i = 0u; i < size; i++) {
            newArr[i] = arr[i];
        }
        allocator.deallocate(arr,size);
        arr = newArr;
        capacity = newCapacity;
    }

public:
    Stack() : capacity(8u), size(0u) {
        arr = allocator.allocate(capacity);
    }

    Stack(const Stack& other) : capacity(other.capacity), size(other.size){
        resize(capacity);
        for (unsigned int i = 0u; i < size; i++) {
            arr[i] = other.arr[i];
        }
    }

    Stack(Stack&& other) noexcept : capacity(other.capacity), size(other.size), arr(other.arr) {
        other.capacity = 0u;
        other.size = 0u;
        other.arr = nullptr;
    }

    ~Stack() {
        clear();
    }

    void push(const T& element) {
        if (size == capacity) {
            resize(capacity + capacity/2);
        }
        arr[size++] = element;
    }

    void pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        size--;
        if (size < capacity / 4u) {
            resize(capacity / 2u);
        }
    }

    T& top() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return arr[size - 1u];
    }

    bool isEmpty() const {
        return size == 0u;
    }

    unsigned int getSize() const {
        return size;
    }

    void clear() noexcept {
        allocator.deallocate(arr,size);
        size = 0u;
        capacity = 4u;
    }

    Stack& operator=(const Stack& other){
        if(this==&other){
            return *this;
        }
        allocator.deallocate(arr,size);
        capacity=other.capacity;
        size=other.size;
        resize(capacity);
        for(size_t i = 0; i < size; ++i){
            arr[i]=other.arr[i];
        }
        return *this;
    }

};

#endif //UNTITLED13_STACK_H
