#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Memory.hpp"
#include "../h/_thread.hpp"
#include "../h/_sem.hpp"
#include "../lib/console.h"

#define ILLEGAL_INSTRUCTION 2

void riscv::syscallHandler(uint64 code, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4) {
    uint64 volatile sstatus = r_sstatus();
    uint64 volatile sepc = r_sepc();
    if (r_scause() == ILLEGAL_INSTRUCTION) {
        printString("ILLEGAL INSTRUCTION:\n");
        printInt(sepc, 16);
        while (true);
    }
    switch (code) {
        case MEM_ALLOC:
            Memory::mem_alloc(arg1);
            break;
        case MEM_FREE:
            Memory::mem_free((void *) arg1);
            break;
        case MEM_GET_FREE_SPACE:
            Memory::mem_get_free_space();
            break;
        case MEM_GET_LARGEST_FREE_BLOCK:
            Memory::mem_get_largest_free_block();
            break;
        case THREAD_CREATE:
            _thread::thread_create((_thread **) arg1, (run_function) arg2, (void *) arg3, (char *) arg4);
            break;
        case THREAD_EXIT:
            _thread::thead_exit();
            break;
        case THREAD_DISPATCH:
            _thread::thread_dispatch();
            break;
		case THREAD_JOIN:
			_thread::thread_join((thread_t) arg1);
			break;
		case WAIT_FOR_CHILDREN:
			_thread::wait_for_children();
			break;
        case SEM_OPEN:
            _sem::sem_open((_sem **) arg1, (int) arg2);
            break;
        case SEM_CLOSE:
            _sem::sem_close((_sem*)arg1);
            break;
        case SEM_WAIT:
            ((_sem*)arg1)->sem_wait();
            break;
        case SEM_SIGNAL:
            ((_sem*)arg1)->sem_signal();
            break;
        case GETC:
            __getc();
            break;
        case PUTC:
            __putc(arg1);
            break;
    }
    w_sstatus(sstatus);
    w_sepc(sepc + 4);
}

void riscv::timerHandler() {
    mc_sip(SIP_SSIE);
}

void riscv::illegalReadHandler() {
    printString("ILLEGAL READ:\n");
    printInt(r_sepc(), 16);
    while (true);
}

void riscv::illegalWriteHandler() {
    printString("ILLEGAL WRITE:\n");
    printInt(r_sepc(), 16);
    while (true);
}

void riscv::popSppSpie() {
    riscv::mc_sstatus(riscv::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}