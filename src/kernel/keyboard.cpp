#include <kernel/keyboard.h>
#include <kernel/io.h>
#include <kernel/vga.h>
#include <kernel/interrupts.h>

unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

extern volatile bool threads_paused;

#define KB_BUFFER_SIZE 256
static char kb_buffer[KB_BUFFER_SIZE];
static int kb_head = 0, kb_tail = 0;

extern "C" struct regs* keyboard_handler(struct regs *r) {
    unsigned char scancode;
    scancode = inb(0x60);

    if (scancode & 0x80) {
        // Key released
    } else {
        // Key pressed
        char c = kbdus[scancode];
        
        // Immediate visual feedback to prove hardware is working
        if (c != 0) {
            terminal_putentryat(c, vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE), 0, 24);
        }

        if (c == 'p' || c == 'P') {
            threads_paused = !threads_paused;
        } else if (c != 0) {
            // Push to circular buffer
            if (((kb_head + 1) % KB_BUFFER_SIZE) != (kb_tail % KB_BUFFER_SIZE)) {
                kb_buffer[kb_head % KB_BUFFER_SIZE] = c;
                kb_head++;
            }
        }
    }
    return r;
}

extern "C" char keyboard_getchar() {
    if (kb_tail == kb_head) return 0;  // empty
    char c = kb_buffer[kb_tail % KB_BUFFER_SIZE];
    kb_tail++;
    return c;
}

extern "C" void keyboard_initialize() {
    irq_install_handler(1, keyboard_handler);
}
