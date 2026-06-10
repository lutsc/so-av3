CROSS = riscv64-unknown-elf-

CC = $(CROSS)gcc
LD = $(CROSS)ld

CFLAGS = \
	-march=rv64gc -mabi=lp64 \
	-mcmodel=medany \
	-ffreestanding \
	-nostdlib \
	-nostartfiles \
	-fno-stack-protector \
	-Wall

OBJS = \
	start.o \
	context.o \
	main.o \
	uart.o \
	memory.o \
	task.o \
	scheduler.o \
	trap_entry.o \
	timer.o \
	trap.o 

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) -T linker.ld $(OBJS) -o kernel.elf

%.o: boot/%.S
	$(CC) $(CFLAGS) -c $<

%.o: kernel/%.c
	$(CC) $(CFLAGS) -Iinclude -c $<

%.o: kernel/%.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o kernel.elf

run: kernel.elf
	qemu-system-riscv64 \
		-machine virt \
		-m 128M \
		-nographic \
		-bios default \
		-kernel kernel.elf
