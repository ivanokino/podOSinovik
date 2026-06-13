section .text
global _start

extern kernel_main
_start:

lea rsp, [rel stack_top]

;from microsoft abi to system V
mov rdi, rcx
mov rsi, rdx
mov rdx, r8
mov rcx, r9
call kernel_main
;;;;;;;;;;;;;;;
cli
hlt
jmp $
;;;;;;;;;;;;;;;

section .bss
    align 16
stack_bottom:
    resb 65536
stack_top:  