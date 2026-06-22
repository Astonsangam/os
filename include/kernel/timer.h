#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <kernel/interrupts.h>

#ifdef __cplusplus
extern "C" {
#endif

void timer_initialize(uint32_t frequency);
uint32_t timer_get_ticks();
struct regs* timer_handler(struct regs *r);

#ifdef __cplusplus
}
#endif

#endif
