#ifndef BLOCK_H
#define BLOCK_H 1
#include <stdint.h>

int block_read(uint32_t block, void *buffer);

int block_write(uint32_t block, const void *buffer);

#endif