#ifndef OS_MEMORY_HPP
#define OS_MEMORY_HPP

#include "../lib/hw.h"

class Memory {
    struct MemoryBlock {
        size_t size;
        MemoryBlock *next;
        MemoryBlock *prev;
    };

public:
    static void init();

    static void *mem_alloc(size_t size);

    static int mem_free(void *ptr);

    static size_t mem_get_free_space();

    static size_t mem_get_largest_free_block();

private:

    static void coalesce(MemoryBlock *first, MemoryBlock *second);

    static MemoryBlock *head;
};


#endif
