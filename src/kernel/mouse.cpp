#include <kernel/mouse.h>
#include <kernel/io.h>
#include <kernel/vga.h>
#include <kernel/interrupts.h>

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];
int mouse_x = 40, mouse_y = 12;

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout--) {
            if ((inb(0x64) & 1) == 1) return;
        }
    } else {
        while (timeout--) {
            if ((inb(0x64) & 2) == 0) return;
        }
    }
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

extern "C" struct regs* mouse_handler(struct regs *r) {
    mouse_byte[mouse_cycle++] = inb(0x60);

    if (mouse_cycle == 3) {
        mouse_cycle = 0;
        // Simple indicator that mouse is moving
        terminal_putentryat('M', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED), 79, 0);
    }
    return r;
}

extern "C" void mouse_initialize() {
    uint8_t status;

    mouse_wait(1);
    outb(0x64, 0xA8); // Enable auxiliary device

    mouse_wait(1);
    outb(0x64, 0x20); // Get status byte
    mouse_wait(0);
    status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60); // Set status byte
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6); // Use default settings
    mouse_read();

    mouse_write(0xF4); // Enable data reporting
    mouse_read();

    irq_install_handler(12, mouse_handler);
}
