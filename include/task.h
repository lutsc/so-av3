#ifndef TASK_H
#define TASK_H 1
#include <stdint.h>
#define MAX_TASKS 8

typedef enum{
  READY, RUNNING, BLOCKED, SUSPENDED  
} STATE;

typedef struct{
  uint64_t regs[31];
  uint64_t sepc;

  void (*entry)(void);
  
  int priority;
  STATE state;
  // int state;

  uint8_t *stack;
  uint64_t stack_size;
} TCB;

extern TCB tasks[MAX_TASKS];
extern int task_count;

void xTaskCreate(void (*task)(void), uint64_t stack_size, int priority);

#endif
