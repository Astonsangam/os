#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <kernel/interrupts.h>

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_initialize();
char keyboard_getchar();
struct regs* keyboard_handler(struct regs *r);

#ifdef __cplusplus
}
#endif

#endif
