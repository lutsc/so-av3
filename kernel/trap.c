#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

void trap_handler(uint64_t *frame) {
  uint64_t scause;

  // lê CSR scause e verifica causa do trap
  asm volatile(
    "csrr %0, scause"
    : "=r"(scause)
  );

  // bit mais signifativo (63) = 1 significa que teve interrupt
  // bits restantes (62 a 0) = motivo do interrupt
  // se motivo do interrupt = 5, é o interrupt de Supervisor Timer Interrupt, procede
  // passa tempo, troca contexto e volta pro trap_entry
  if ((scause >> 63) && ((scause & 0x3F) == 5)) {
    timer_next();
    schedule_from_trap(frame);
    return;
  }

  // trap não bateu condições
  uart_print("Unhandled trap\n");

  while(1);
}