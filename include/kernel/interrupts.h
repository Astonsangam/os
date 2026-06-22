#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

#ifdef __cplusplus
extern "C" {
#endif

void isr_handler(struct regs* r);
uint32_t irq_handler(struct regs* r);

void interrupts_initialize();
void irq_install_handler(int irq, struct regs* (*handler)(struct regs *r));

#ifdef __cplusplus
}
#endif

#endif
