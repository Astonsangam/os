#ifndef MOUSE_H
#define MOUSE_H

#include <kernel/interrupts.h>

#ifdef __cplusplus
extern "C" {
#endif

void mouse_initialize();
struct regs* mouse_handler(struct regs *r);

#ifdef __cplusplus
}
#endif

#endif
