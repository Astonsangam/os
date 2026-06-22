#include <kernel/vga.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/interrupts.h>
#include <kernel/keyboard.h>
#include <kernel/mouse.h>
#include <kernel/timer.h>
#include <kernel/scheduler.h>

volatile bool threads_paused = false;

void display_banner() {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    terminal_writestring("  ____           _                 _               ____   ____  \n");
    terminal_writestring(" |  _ \\ __ _  __| | __ _ _ __   __| | __ _ ___    / __ \\ / ___| \n");
    terminal_writestring(" | |_) / _` |/ _` |/ _` | '_ \\ / _` |/ _` / __|  / /  \\ \\\\___ \\ \n");
    terminal_writestring(" |  __/ (_| | (_| | (_| | | | | (_| | (_| \\__ \\  \\ \\__/ / ___| |\n");
    terminal_writestring(" |_|   \\__,_|\\__,_|\\__,_|_| |_|\\__,_|\\__,_|___/   \\____/ |____/ \n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("\n");
}

extern "C" void thread1() {
    while (1) {
        if (!threads_paused) {
            terminal_writestring("Thread 1 is running...\n");
        }
        for(int i=0; i<30000000; i++) asm volatile("nop");
    }
}

extern "C" void thread2() {
    while (1) {
        if (!threads_paused) {
            terminal_writestring("Thread 2 is running...\n");
        }
        for(int i=0; i<30000000; i++) asm volatile("nop");
    }
}

extern "C" void kernel_main(unsigned int magic_number, void* multiboot_structure) {
    (void)magic_number;
    (void)multiboot_structure;
    
    terminal_initialize();
    display_banner();
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK));
    terminal_writestring("Welcome to PadandasOS!\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("Booting...\n");

    gdt_initialize();
    idt_initialize();
    interrupts_initialize();

    terminal_writestring("Initializing Drivers...\n");
    timer_initialize(100); // 100 Hz
    keyboard_initialize();
    mouse_initialize();

    terminal_writestring("Initializing Scheduler...\n");
    scheduler_initialize();
    scheduler_add_thread(thread1);
    scheduler_add_thread(thread2);

    asm volatile("sti");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    terminal_writestring("PadandasOS System Ready. Press 'P' to pause threads.\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

    while (1) {
        asm volatile("hlt");
    }
}
