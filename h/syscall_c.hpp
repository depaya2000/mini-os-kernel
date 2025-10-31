#ifndef OS_SYSCALL_C_HPP
#define OS_SYSCALL_C_HPP

#include "../lib/hw.h"

#define MEM_ALLOC 0x01
#define MEM_FREE 0x02
#define MEM_GET_FREE_SPACE 0x03
#define MEM_GET_LARGEST_FREE_BLOCK 0X04

#define THREAD_CREATE 0X11
#define THREAD_EXIT 0x12
#define THREAD_DISPATCH 0x13
#define THREAD_JOIN 0x14
#define WAIT_FOR_CHILDREN 0x15

#define SEM_OPEN 0x21
#define SEM_CLOSE 0x22
#define SEM_WAIT 0X23
#define SEM_SIGNAL 0x24

#define TIME_SLEEP 0x31

#define GETC 0x41
#define PUTC 0x51

void *mem_alloc(size_t size);
int mem_free(void *ptr);
size_t mem_get_free_space();
size_t mem_get_largest_free_block();

class _thread;
typedef _thread *thread_t;
int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t id);
void wait_for_children();

class _sem;
typedef _sem* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc();
void putc(char);
#endif
