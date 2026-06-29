#ifndef MEMORY_H
#define MEMORY_H 1
#include <stdint.h>

void memory_init(void);

void *kmalloc(uint64_t size);

// void kfree(void *ptr);

// uint64_t memory_free(void);

uint64_t memory_used(void);

#endif
