#ifndef OS_SCHEDULER_HPP
#define OS_SCHEDULER_HPP
#include "buffer.hpp"

class _thread;

class Scheduler {
public:
    static _thread*get();

    static void put(_thread*);
private:
    static buffer<_thread> threads;
};

#endif
