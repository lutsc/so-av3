#include "timer.h"
#include <uart.h>

static uint64_t tick_interval = 100000;

// Especificação do SBI
#define SBI_EID 0x00 // Extension ID 

static inline void sbi_set_timer(uint64_t time){
  register uint64_t a0 asm("a0") = time;
  register uint64_t a7 asm("a7") = SBI_EID; // legacy set_timer EID
  asm volatile(
    "ecall"
    : "+r"(a0) // leitura e escrita do valor, passa tempo e pode retornar erro no reg. a0
    : "r"(a7)  // só leitura, passa extensão da função nos registrador a7
    : "memory"
  );
}

void timer_next(void){
	uint64_t now;

  // pseudo-instrução que lê o valor de tempo do CSR (Control and Status Register) 
  asm volatile("rdtime %0" : "=r"(now) : : "memory"); 
  
  // tempo atual + intervalo
  sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval){
  if (interval != 0)
    tick_interval = interval;

  timer_next();

  // ativa o 5° bit do reg. sie é o stie, habilitando interrupção do timer 
  // basicamente se o timer disparar, o modo supervisor interrompe o código
  asm volatile("csrs sie, %0" :: "r"(1 << 5));
  
  // ativa o 1° bit do reg. sstatus, habilitando interrupções em geral
  // tem que ativar pra qualquer interrupção acontecer
  asm volatile("csrs sstatus, %0" :: "r"(1 << 1));
}
