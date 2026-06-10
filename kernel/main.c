#include "uart.h"
#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"

extern void uart_print(const char*);
extern void trap_entry(void);

void task1(void){
	uart_print("Task 1 running\n");
	yield();
}

void task2(void){
	uart_print("Task 2 running\n");
	yield();
}

void kernel_main(){
	memory_init();

	xTaskCreate(task1, 2048, 1);
	xTaskCreate(task2, 2048, 1);

	asm volatile("csrw stvec, %0" :: "r"(trap_entry));

	timer_init(100000);

	scheduler_start();
	while (1);
}
