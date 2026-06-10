#include "scheduler.h"
#include "task.h"
#include <stdint.h>

extern TCB tasks[];
static TCB kernel_context;
extern int task_count;

extern void context_switch(void*, void*);

static int current = 0;

static int round_robin(void){
  if (task_count == 0)
    return 0;
  return (current + 1) % task_count;
}

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo){
  if(algo)
    current_algo = algo;
}

int scheduler_current_task(void){
  return current;
}

void yield(void){
  if (task_count < 2)
    return;

  int prev = current;
  int next = current_algo();

  if (prev == next)
    return;

  current = next;
  context_switch(tasks[prev].regs, tasks[next].regs);
}

void schedule_from_trap(uint64_t *frame){
  if (task_count < 2)
    return;

  int prev = current;
  int next = current_algo();

  if (prev == next)
    return;

  for (int i = 0; i < 31; i++)
    tasks[prev].regs[i] = frame[i];

  asm volatile("csrr %0, sepc": "=r"(tasks[prev].sepc));

  current = next;

  for (int i = 0; i < 31; i++)
    frame[i] = tasks[next].regs[i];

  asm volatile("csrw sepc, %0":: "r"(tasks[next].sepc));
}

void scheduler_start(void){
  if (task_count == 0)
    return;
  current = 0;
  context_switch(&kernel_context.regs, tasks[0].regs);
}