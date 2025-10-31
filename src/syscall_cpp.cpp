#include "../h/syscall_cpp.hpp"

void *operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void *ptr) {
    mem_free(ptr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    this->body = nullptr;
    this->arg = nullptr;
}

Thread::~Thread() {

}

int Thread::start() {
    return thread_create(&myHandle, threadWrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::join(){
	thread_join(this->myHandle);
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::threadWrapper(void *arg) {
    Thread *thread = (Thread*)arg;
    if (thread->body) {
        thread->body(thread->arg);
    } else {
        thread->run();
    }
}

Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

void PeriodicThread::terminate() {

}

PeriodicThread::PeriodicThread(time_t period) {

}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}