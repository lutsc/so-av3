#include "block.h"
#include "uart.h"

// Disco virtual em memória
static uint8_t disk[DISK_SIZE];

int block_read(uint32_t block, void *buffer) {
  if (block >= (DISK_SIZE / BLOCK_SIZE) || buffer == NULL){
    uart_print("Erro na leitura de bloco ou buffer vazio.");
    return -1;
  }

  uint32_t offset = (block * BLOCK_SIZE);
  uint8_t *destination = (uint8_t *)buffer;

  for (uint32_t i = 0; i < BLOCK_SIZE; i++)
    destination[i] = disk[offset + i];

  return 0;
}

int block_write(uint32_t block, const void *buffer) {
  if (block >= (DISK_SIZE / BLOCK_SIZE) || buffer == NULL){
    uart_print("Erro na escrita de bloco ou buffer vazio.");
    return -1;
  }

  uint32_t offset = (block * BLOCK_SIZE);
  uint8_t *source = (uint8_t *)buffer;

  for (uint32_t i = 0; i < BLOCK_SIZE; i++)
    disk[offset + i] = source[i];

  return 0;
}