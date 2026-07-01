#include "uart.h"
#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"
#include "fs.h"

extern void uart_print(const char*);
extern void trap_entry(void);

void task1(void){
	while (1)
		uart_print("Task 1 running\n");
}

void task2(void){
	while (1)
		uart_print("Task 2 running\n");
}

void kernel_main(){
	// memory_init();

	uart_print("\n=== Kernel ===\n");

	// xTaskCreate(task1, 2048, 1);
	// xTaskCreate(task2, 2048, 1);

	// asm volatile("csrw stvec, %0" :: "r"(trap_entry));

	// timer_init(100000);

	// scheduler_start();
	// while (1);

	fs_init();
	fs_create("notas.txt");
	fs_create("notas.txt");

	fs_delete("notas.txt");

	fs_create("notas.txt");

	// int fd = fs_open("notas.txt");
	// fs_write(fd, "Sistemas Operacionais", 22);

	// char buffer[64];
	// fs_read(fd, buffer, 22);

	// fs_close(fd);
}
