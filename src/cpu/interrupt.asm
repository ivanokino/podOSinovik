[extern isr_handler]
[extern irq_handler]



isr_common_stub:
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
    push rbp
    push r8
    push r9
    push r10
    push r11
  
    push r12     
    push r13     
    push r14      
    push r15 
    mov rcx,rsp ;cuz void isr_handler(registers_t r)
    call isr_handler
    
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    
    add rsp, 16
    iretq
    
irq_common_stub:
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12     
    push r13     
    push r14     
    push r15     
    
    mov rcx, rsp 
    call irq_handler
    
    pop r15      
    pop r14      
    pop r13      
    pop r12      
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    
    add rsp, 16
    iretq

%macro ISR_NOERR 1
    global isr%1
    isr%1:
        cli
        push qword 0
        push qword %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
    global isr%1
    isr%1:
        cli
        push qword %1
        jmp isr_common_stub
%endmacro

%macro IRQ 1
    global irq%1
    irq%1:
        cli
        push qword 0
        push qword %1+32
        jmp irq_common_stub

%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
