#ifndef OS__THREAD_HPP
#define OS__THREAD_HPP

#include "../lib/hw.h"
#include "syscall_c.hpp"

#define RUNNABLE 0
#define BLOCKED 1
#define FINISHED 2

#define SIGNALLED 0
#define CLOSED 1

using run_function = void (*)(void *);

class _thread {
public:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    _thread(run_function run, void *arg, char *stack_space);

    static int thread_create(_thread** handle, run_function run, void *arg, char *stack_space);

    static int thead_exit();

    static void thread_dispatch();
	
	static void thread_join(thread_t id);
	
	static void wait_for_children();

    int get_state();

    void set_state(int state);

    int get_sem_state();

    void set_sem_state(int sem_state);

    static _thread *get_running() {
        return running;
    }
    static void thread_start();
private:
    run_function run;
    void *arg;
    void *stack_space;
    int state;
    Context context;
    int sem_state;
    static _thread *running;
	sem_t joiner;
	thread_t children[100];
	int children_size = 0;
};


#endif
