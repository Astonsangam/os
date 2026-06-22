#include <kernel/scheduler.h>
#include <kernel/vga.h>

#define MAX_THREADS 10
#define STACK_SIZE 4096

static struct thread threads[MAX_THREADS];
static uint8_t thread_stacks[MAX_THREADS][STACK_SIZE];
static int thread_count = 0;
static int current_thread_idx = 0;

extern "C" void scheduler_initialize() {
    // The main kernel thread is thread 0
    threads[0].id = 0;
    threads[0].next = &threads[0];
    thread_count = 1;
}

extern "C" void scheduler_add_thread(void (*entry_point)()) {
    if (thread_count >= MAX_THREADS) return;

    struct thread* t = &threads[thread_count];
    t->id = thread_count;
    t->stack_base = (uint32_t)&thread_stacks[thread_count];
    
    // Set up the stack as if it was interrupted
    uint32_t* stack = (uint32_t*)(t->stack_base + STACK_SIZE);
    
    // IRET frame
    *(--stack) = 0x202;         // EFLAGS (Interrupts enabled)
    *(--stack) = 0x08;          // CS
    *(--stack) = (uint32_t)entry_point; // EIP

    // Interrupt/Exception info (pushed by macro/stub)
    *(--stack) = 0;             // int_no
    *(--stack) = 0;             // err_code
    
    // Push dummy registers for pusha
    *(--stack) = 0; // EAX
    *(--stack) = 0; // ECX
    *(--stack) = 0; // EDX
    *(--stack) = 0; // EBX
    *(--stack) = 0; // ESP (ignored by popa)
    *(--stack) = 0; // EBP
    *(--stack) = 0; // ESI
    *(--stack) = 0; // EDI
    
    // Push segment registers
    *(--stack) = 0x10; // GS
    *(--stack) = 0x10; // FS
    *(--stack) = 0x10; // ES
    *(--stack) = 0x10; // DS

    t->esp = (uint32_t)stack;

    // Link into the circular list
    t->next = threads[0].next;
    threads[0].next = t;

    thread_count++;
}

volatile bool scheduler_enabled = true;

extern "C" struct regs* scheduler_switch(struct regs* r) {
    if (thread_count <= 1 || !scheduler_enabled) return r;

    // Save current thread's ESP
    threads[current_thread_idx].esp = (uint32_t)r;

    // Pick next thread, skipping index 0 (kernel main)
    current_thread_idx++;
    if (current_thread_idx >= thread_count) {
        current_thread_idx = 1; // Start from thread 1
    }

    // Return new thread's ESP (which is the pointer to its regs struct)
    return (struct regs*)threads[current_thread_idx].esp;
}
