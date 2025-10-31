#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Memory.hpp"

extern "C" void interrupt();

extern void userMain();

_sem* main_sem;

void userMainWrapper(void *) {
    userMain();
    sem_signal(main_sem);
}

int main() {
    Memory::init();
    riscv::w_stvec((uint64)&interrupt | 0x1);
    sem_open(&main_sem, 0);
    thread_t main_handle, user_main_handle;
    thread_create(&main_handle, nullptr, nullptr);
    thread_create(&user_main_handle, userMainWrapper, nullptr);
    sem_wait(main_sem);
    return 0;
}