#ifndef OS_BUFFER_HPP
#define OS_BUFFER_HPP
#include "../test/printing.hpp"

template<typename T> class buffer {
public:
    buffer();

    T* get();

    void put(T*);

    bool isEmpty();
private:
    static constexpr int CAPACITY = 128;
    T* data[CAPACITY];
    int front;
    int back;
    int size;
};

template<typename T>
bool buffer<T>::isEmpty() {
    return size == 0;
}

template<typename T>
void buffer<T>::put(T *elem) {
    data[back] = elem;
    back = (back + 1) % CAPACITY;
    size += 1;
}

template<typename T>
T *buffer<T>::get() {
    if (isEmpty()) return nullptr;
    T* ret = data[front];
    front = (front + 1) % CAPACITY;
    size -= 1;
    return ret;
}

template<typename T>
buffer<T>::buffer() {
    front = 0;
    back = 0;
    size = 0;
}

#endif
