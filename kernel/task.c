#include "task.h"
#include "memory.h"

TCB tasks[MAX_TASKS];
int task_count = 0;

void xTaskCreate(void (*task)(void), uint64_t stack_size, int priority){
  if (task_count >= MAX_TASKS)
    return;

  TCB *t = &tasks[task_count++];

  t->entry = task;
  t->priority = priority;
  t->state = READY;
  t->stack_size = stack_size;
  t->stack = (uint8_t *)kmalloc(stack_size);
  
  if (!t->stack)
    return;

  uint64_t *sp = (uint64_t *)((uint64_t)(t->stack + stack_size) & ~15ULL);

  for (int i = 0; i < 32; i++) {
    t->regs[i] = 0;
  }

  register uint64_t current_gp asm("gp");
  register uint64_t current_tp asm("tp");

  // Alinhado com o novo context.S:
  t->regs[0] = (uint64_t) task;        // 0(a0)   -> ra
  t->regs[1] = (uint64_t) sp;          // 8(a0)   -> sp
  t->regs[2] = (uint64_t) sp;          // 16(a0)  -> s0 (frame pointer inicial aponta pro stack)
  t->regs[14] = current_gp;            // 112(a0) -> gp
  t->regs[15] = current_tp;            // 120(a0) -> tp

  t->sepc = (uint64_t) task;
}
