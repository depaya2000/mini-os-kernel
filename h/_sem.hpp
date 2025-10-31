#ifndef OS__SEM_HPP
#define OS__SEM_HPP

#include "buffer.hpp"
#include "_thread.hpp"

class _sem {
public:
    _sem(int init);

    static int sem_open(_sem **handle, int init);

    static int sem_close(_sem *handle);

    int sem_wait();

    int sem_signal();

private:
    int val;
    buffer<_thread> blocked;
};

#endif
