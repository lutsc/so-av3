#include "uart.h"
#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"
#include "fs.h"
#include "string.h"

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
	uart_print("\n=== Kernel ===\n\n");
	
	// memory_init();
	// xTaskCreate(task1, 2048, 1);
	// xTaskCreate(task2, 2048, 1);

	// asm volatile("csrw stvec, %0" :: "r"(trap_entry));
	// timer_init(100000);
	// scheduler_start();
	// while (1);

	fs_init();
	uart_print("\n");

	// Não permite arquivos iguais
	fs_create("notas.txt");
	fs_create("notas.txt");
	uart_print("\n");

	// Consegue depois de deletado
	fs_delete("notas.txt");
	fs_create("notas.txt");
	uart_print("\n");

	// Escrita
	int fd = fs_open("notas.txt");
	fs_write(fd, "Sistemas Operacionais\n", 23);
	fs_close(fd);

	// Leitura
	fd = fs_open("notas.txt");
	char buffer[64];
	fs_read(fd, buffer, 23);
	uart_print(buffer);
	fs_close(fd);
	uart_print("\n");

	fs_delete("notas.txt");
	uart_print("\n");

	// Testes
	fs_create("teste1.txt");
	fd = fs_open("teste1.txt");
	// fs_write(fd, "teste1\n", 1024*1024);
	fs_write(fd, "teste1\n", 1024*1018);
	fs_close(fd);

	fs_delete("teste1.txt");
	uart_print("\n");

	fs_create("teste2.txt");
	fd = fs_open("teste2.txt");
	fs_write(fd, "teste2\n", 64);
	fs_close(fd);

	fs_create("teste3.txt");
	fd = fs_open("teste3.txt");
	fs_write(fd, "teste3\n", 64);
	fs_close(fd);

}
