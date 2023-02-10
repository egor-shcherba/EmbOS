CC = cc
AS = as
LD = ld

CFLAGS = \
	-Wall -Wextra -pedantic -O2 -std=c11 \
	-m32 -nostdinc -mgeneral-regs-only \
	-ffreestanding -fno-stack-protector \
	-fno-omit-frame-pointer \
	-fno-common \
	-I ./inc -I./

ASFLAGS = \
	--32

LDFLAGS = \
	-static -nostdlib -O2 \
	-T ./linker.lds

SRC = \
	startup/boot.S \
	main.c  \
	klib/string.c \
	klib/vsprintf.c \
	debug/qemu.c \
	sys/gdt.c \
	sys/idt.c \
	sys/exception.c \
	sys/isr.S \
	sys/pic.c \
	sys/pit.c \
	mem/heap.c \
	driver/uart.c \
	system/printf.c \
	system/syscall.c \
	system/sched.c \
	system/mutex.c \
	system/timer.c \
	bin/init.c \

OBJ = $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SRC)))

BINARY = kernel.elf

QEMU = qemu-system-x86_64

QEMU_FLAGS = -m 32 -debugcon stdio

all: clean $(OBJ)
	$(LD) $(LDFLAGS) -o $(BINARY) $(OBJ)

qemu:
	$(QEMU) $(QEMU_FLAGS) -kernel $(BINARY)

%.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJ)
