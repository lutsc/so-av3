#include "timer.h"

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
    if (interval != 0)
        tick_interval = interval;

    /* STIE = Supervisor Timer Interrupt Enable */
    asm volatile(
        "csrs sie, %0"
        :
        : "r"(1UL << 5)
    );

    /* SIE = Global Supervisor Interrupt Enable */
    asm volatile(
        "csrsi sstatus, 2"
    );

    timer_next();
}
