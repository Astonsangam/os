# Compiler/Linker settings
AS = nasm
CC = gcc
CXX = g++
LD = ld
OBJCOPY = objcopy
QEMU = "C:/Program Files/qemu/qemu-system-i386.exe"

# Flags
ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
CXXFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude
LDFLAGS = -mi386pe -T linker.ld -nostdlib

# Objects
OBJS = src/boot/loader.o \
       src/boot/gdt_flush.o \
       src/boot/idt_load.o \
       src/boot/interrupts.o \
       src/kernel/kernel.o \
       src/kernel/vga.o \
       src/kernel/gdt.o \
       src/kernel/idt.o \
       src/kernel/interrupts.o \
       src/kernel/keyboard.o \
       src/kernel/mouse.o \
       src/kernel/timer.o \
       src/kernel/scheduler.o

# Targets
kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.pe $(OBJS)
	$(OBJCOPY) -O elf32-i386 kernel.pe $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /f $(subst /,\,$(OBJS)) kernel.bin kernel.pe

run: kernel.bin
	$(QEMU) -kernel kernel.bin
