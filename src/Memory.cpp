#include "../h/Memory.hpp"

Memory::MemoryBlock *Memory::head = nullptr;

void Memory::init() {
    head = (MemoryBlock*) HEAP_START_ADDR;
    head->next = head->prev = nullptr;
    head->size = (uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR - sizeof(MemoryBlock);
}

void *Memory::mem_alloc(size_t size) {
    size *= MEM_BLOCK_SIZE;
    MemoryBlock *curr;
    for (curr = head; curr; curr = curr->next) {
        if (curr->size >= size) {
            break;
        }
    }
    if (curr == nullptr) return nullptr;
    if (curr->size < MEM_BLOCK_SIZE + sizeof(MemoryBlock) + size) {
        if (curr->prev) curr->prev->next = curr->next;
        else head = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
    } else {
        MemoryBlock *newBlock = (MemoryBlock *) ((uint64) curr + size + sizeof(MemoryBlock));
        newBlock->next = curr->next;
        if (curr->next) curr->next->prev = newBlock;
        newBlock->prev = curr->prev;
        if (curr->prev) curr->prev->next = newBlock;
        else head = newBlock;
        newBlock->size = curr->size - size - sizeof(MemoryBlock);
        curr->size = size;
    }
    return (void *) ((uint64) curr + sizeof(MemoryBlock));
}

int Memory::mem_free(void *ptr) {
    MemoryBlock *arg = (MemoryBlock *) ((uint64) ptr - sizeof(MemoryBlock));
    MemoryBlock *curr;
    if (arg < head) {
        arg->next = head;
        head->prev = arg;
        arg->prev = nullptr;
        coalesce(arg, head);
        head = arg;
        return 0;
    }
    for (curr = head; curr->next; curr = curr->next) {
        if (curr < arg and curr->next > arg) break;
    }
    if (not curr->next) {
        curr->next = arg;
        arg->prev = curr;
        arg->next = nullptr;
        coalesce(curr, arg);
    } else {
        curr->next->prev = arg;
        arg->next = curr->next;
        curr->next = arg;
        arg->prev = curr;
        coalesce(arg, arg->next);
        coalesce(curr, arg);
    }
    return 0;
}

size_t Memory::mem_get_free_space() {
    size_t free_space = 0;
    for (MemoryBlock *curr = head; curr; curr = curr->next) {
        free_space += curr->size;
    }
    return free_space;
}

size_t Memory::mem_get_largest_free_block() {
    size_t largest = head->size;
    for(MemoryBlock *curr = head->next; curr; curr = curr->next) {
        if (curr->size > largest) largest = curr->size;
    }
    return largest;
}

void Memory::coalesce(MemoryBlock *first, MemoryBlock *second) {
    if (not first or not second) return;
    if ((uint64) first + sizeof(MemoryBlock) + first->size == (uint64) second) {
        first->size += second->size + sizeof(MemoryBlock);
        first->next = second->next;
        if(second->next) second->next->prev = first;
    }
}