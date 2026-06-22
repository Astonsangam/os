#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <kernel/interrupts.h>

struct thread {
    int id;
    uint32_t esp;
    uint32_t stack_base;
    struct thread* next;
};

#ifdef __cplusplus
extern "C" {
#endif

void scheduler_initialize();
void scheduler_add_thread(void (*entry_point)());
struct regs* scheduler_switch(struct regs* r);

#ifdef __cplusplus
}
#endif

#endif
