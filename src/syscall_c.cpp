#include "../h/syscall_c.hpp"

extern "C" int call(uint64, uint64, uint64, uint64, uint64);

uint64 put_args(uint64 code, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0, uint64 arg4 = 0) {
    return call(code, arg1, arg2, arg3, arg4);
}

void *mem_alloc(size_t size) {
    if (size == 0) return nullptr;
    size = (size - 1) / MEM_BLOCK_SIZE + 1;
    return (void *) put_args(MEM_ALLOC, size);
}

int mem_free(void *ptr) {
    return put_args(MEM_FREE, (uint64) ptr);
}

size_t mem_get_free_space() {
    return put_args(MEM_GET_FREE_SPACE);
}

size_t mem_get_largest_free_block() {
    return put_args(MEM_GET_LARGEST_FREE_BLOCK);
}

int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    void *stack_space = nullptr;
    if (start_routine) {
        stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    }
    return put_args(THREAD_CREATE, (uint64)handle, (uint64) start_routine, (uint64) arg, (uint64) stack_space);
}

int thread_exit() {
    return put_args(THREAD_EXIT);
}

void thread_dispatch() {
    put_args(THREAD_DISPATCH);
}

void thread_join(thread_t id){
	put_args(THREAD_JOIN, (uint64) id);
}

void wait_for_children(){
	put_args(WAIT_FOR_CHILDREN);
}

int sem_open(sem_t *handle, unsigned init) {
    return put_args(SEM_OPEN, (uint64)handle, init);
}

int sem_close(sem_t handle) {
    return put_args(SEM_CLOSE, (uint64) handle);
}

int sem_wait(sem_t id) {
    return put_args(SEM_WAIT, (uint64)id);
}

int sem_signal(sem_t id) {
    return put_args(SEM_SIGNAL, (uint64)id);
}

int time_sleep(time_t) {
    return 0;
}

char getc() {
    return put_args(GETC);
}

void putc(char c) {
    put_args(PUTC, c);
}