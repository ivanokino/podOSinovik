section .text
global _start

extern kernel_main
_start:

mov rsp, stack_top

call kernel_main
;;;;;;;;;;;;;;;
cli
hlt
jmp $
;;;;;;;;;;;;;;;

section .bss
    align 16
stack_bottom:
    resb 16384
stack_top: