; Multiboot constants
MB_MAGIC equ 0x1BADB002
MB_FLAGS equ (1 << 0) | (1 << 1)
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
    align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .text
extern kernel_main
global loader

loader:
    mov esp, stack_top    ; Set up the stack pointer
    
    push ebx              ; Multiboot info structure
    push eax              ; Multiboot magic number
    
    cli                   ; Disable interrupts
    call kernel_main      ; Call our C++ kernel
    
.halt:
    hlt                   ; Halt the CPU
    jmp .halt             ; Loop forever

section .bss
align 16
stack_bottom:
    resb 16384            ; 16 KB stack
stack_top:
