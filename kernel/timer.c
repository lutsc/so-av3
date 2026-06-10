#include "timer.h"
#include <uart.h>

static uint64_t tick_interval = 100000;

/*
 * OpenSBI Legacy Extension
 * SBI_SET_TIMER = 0
 */
static inline void sbi_set_timer(uint64_t time)
{
    register uint64_t a0 asm("a0") = time;
    register uint64_t a7 asm("a7") = 0;

    asm volatile(
        "ecall"
        :
        : "r"(a0), "r"(a7)
        : "memory"
    );
}

void timer_next(void)
{
    uint64_t now;

    asm volatile(
        "rdtime %0"
        : "=r"(now)
    );

    sbi_set_timer(now + tick_interval);
}

void timer_init(uint64_t interval)
{
    uart_print("T1\n");

    if (interval != 0)
        tick_interval = interval;

    uart_print("T2\n");

    asm volatile(
        "csrs sie, %0"
        :
        : "r"(1UL << 5)
    );

    uart_print("T3\n");

    asm volatile(
        "csrsi sstatus, 2"
    );

    uart_print("T4\n");

    timer_next();

    uart_print("T5\n");
}
