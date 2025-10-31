#include "../h/_thread.hpp"
#include "../h/_sem.hpp"
#include "../h/Scheduler.hpp"

_sem::_sem(int init) {
    this->val = init;
}

int _sem::sem_open(_sem **handle, int init) {
    *handle = new _sem(init);
    if (not *handle) return -1;
    return 0;
}

int _sem::sem_close(_sem *handle) {
    while(not handle->blocked.isEmpty()) {
        _thread *next = handle->blocked.get();
        next->set_sem_state(CLOSED);
        next->set_state(RUNNABLE);
        Scheduler::put(next);
    }
    delete handle;
    return 0;
}

int _sem::sem_wait() {
    if (val == 0) {
        _thread::get_running()->set_state(BLOCKED);
        blocked.put(_thread::get_running());
        thread_dispatch();
        if (_thread::get_running()->get_sem_state() == SIGNALLED) {
            return 0;
        } else {
            _thread::get_running()->set_state(SIGNALLED);
            return -1;
        }
    } else {
        val -= 1;
    }
    return 0;
}

int _sem::sem_signal() {
    if (!blocked.isEmpty()) {
        _thread *next = blocked.get();
        next->set_state(RUNNABLE);
        next->set_sem_state(SIGNALLED);
        Scheduler::put(next);
    } else {
        val += 1;
    }
    return 0;
}
