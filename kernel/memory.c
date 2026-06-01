#include "memory.h"
#include <stdint.h>
#define HEAP_SIZE 0x10000

static uint8_t heap[HEAP_SIZE];
static uint64_t heap_top = 0;

void memory_init(void){
  heap_top = 0;
}

void *kmalloc(uint64_t size){
  /* Alinhamento para 8 bytes */
  size = (size + 7) & ~7ULL;
  
  if (heap_top + size > HEAP_SIZE)
    return 0;

  void *ptr = &heap[heap_top];
  heap_top += size;
  
  return ptr;
}

uint64_t memory_used(void){
  return heap_top;
}
