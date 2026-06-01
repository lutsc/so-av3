#ifndef MEMORY_H
#define MEMORY_H 1
#include <stdint.h>

void memory_init(void);
void *kmalloc(uint64_t size);
uint64_t memory_used(void);

#endif
