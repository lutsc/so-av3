#ifndef BLOCK_H
#define BLOCK_H 1
#include <stdint.h>

#define DISK_SIZE (1024*1024) // Tamanho de 1MB
#define BLOCK_SIZE 512 // Qtd. bytes de cada bloco

int block_read(uint32_t block, void *buffer);

int block_write(uint32_t block, const void *buffer);

#endif