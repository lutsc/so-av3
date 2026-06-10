#include "uart.h"
#include "task.h"
#include "scheduler.h"

void task1(void)
{
    while (1)
    {
        uart_print("Task 1\n");

        for (volatile int i = 0; i < 100000; i++);

        yield();
    }
}

void task2(void)
{
    while (1)
    {
        uart_print("Task 2\n");

        for (volatile int i = 0; i < 100000; i++);

        yield();
    }
}

void kernel_main(void)
{
    uart_print("=== Kernel ====\n");

    xTaskCreate(task1, 4096, 1);
    xTaskCreate(task2, 4096, 1);

    scheduler_start();

    while (1);
}
