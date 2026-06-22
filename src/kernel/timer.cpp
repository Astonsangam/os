#include <kernel/timer.h>
#include <kernel/io.h>
#include <kernel/interrupts.h>
#include <kernel/scheduler.h>

uint32_t timer_ticks = 0;

extern "C" struct regs* timer_handler(struct regs *r) {
    timer_ticks++;
    return scheduler_switch(r);
}

extern "C" void timer_initialize(uint32_t frequency) {
    irq_install_handler(0, timer_handler);

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

extern "C" uint32_t timer_get_ticks() {
    return timer_ticks;
}
