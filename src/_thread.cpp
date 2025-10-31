#include "../h/_thread.hpp"
#include "../h/riscv.hpp"
#include "../h/Scheduler.hpp"
#include "../h/_sem.hpp"

extern "C" void context_switch(_thread::Context *, _thread::Context *);

_thread *_thread::running = nullptr;

_thread::_thread(run_function run, void *arg, char *stack_space) {
    this->run = run;
    this->arg = arg;
    this->stack_space = stack_space;
    this->context.ra = (uint64) &_thread::thread_start;
    this->context.sp = (uint64) &stack_space[DEFAULT_STACK_SIZE - 1];
    this->state = RUNNABLE;
    this->sem_state = SIGNALLED;
	sem_open(&this->joiner, 0);
}

int _thread::thread_create(_thread **handle, run_function run, void *arg, char *stack_space) {
    *handle = new _thread(run, arg, stack_space);
    if (*handle == nullptr) return -1;
    if (!running) {
        running = *handle;
    } else {
		running -> children[running -> childrenSize++] = *handle;
        Scheduler::put(*handle);
    }
    return 0;
}

int _thread::thead_exit() {
    running->state = FINISHED;
	sem_close(running->joiner);
    thread_dispatch();
    return -1;
}

void _thread::thread_dispatch() {
    _thread *old = running;
    if (old->state == RUNNABLE) {
        Scheduler::put(old);
    } else if (old->state == FINISHED) {
        mem_free(old->stack_space);
    }
    running = Scheduler::get();
    if (running != old) {
        context_switch(&(old->context), &(running->context));
    }
}

void _thread::thread_join(thread_t id){
	if(id -> get_state() == FINISHED) return;
	sem_wait(id->joiner);
}

void wait_for_children(){
	for(int i = 0; i < running-> childrenSize; i++){
		thread_join(running -> children[i]);
	

int _thread::get_state() {
    return this->state;
}

void _thread::set_state(int state) {
    this->state = state;
}

int _thread::get_sem_state() {
    return this->sem_state;
}

void _thread::set_sem_state(int sem_state) {
    this->sem_state = sem_state;
}

void _thread::thread_start() {
    riscv::popSppSpie();
    running->run(running->arg);
    ::thread_exit();
}
