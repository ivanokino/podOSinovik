#include "isr_irq.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/APIC.h"

MS_ABI void int_to_ascii(int n, char str[]){
    int i, sign;
    if((sign=n)<0) n = -n;
    i=0;
    do{
        str[i++]=n%10+'0';
    }while ((n/=10)>0);
    if(sign<0)str[i++]='-';
    str[i]='\0';

    int lenght = 0;
    while(str[lenght]!='\0')lenght++;

    for(int i=0;i<lenght/2;i++){
        char temp = str[i];
        str[i]=str[lenght-i-1];
        str[lenght-i-1]=temp;
    }
}
isr_t interrupt_handlers[256];


MS_ABI void isr_install() {
    set_idt_gate(0, (u64)isr0);
    set_idt_gate(1, (u64)isr1);
    set_idt_gate(2, (u64)isr2);
    set_idt_gate(3, (u64)isr3);
    set_idt_gate(4, (u64)isr4);
    set_idt_gate(5, (u64)isr5);
    set_idt_gate(6, (u64)isr6);
    set_idt_gate(7, (u64)isr7);
    set_idt_gate(8, (u64)isr8);
    set_idt_gate(9, (u64)isr9);
    set_idt_gate(10, (u64)isr10);
    set_idt_gate(11, (u64)isr11);
    set_idt_gate(12, (u64)isr12);
    set_idt_gate(13, (u64)isr13);
    set_idt_gate(14, (u64)isr14);
    set_idt_gate(15, (u64)isr15);
    set_idt_gate(16, (u64)isr16);
    set_idt_gate(17, (u64)isr17);
    set_idt_gate(18, (u64)isr18);
    set_idt_gate(19, (u64)isr19);
    set_idt_gate(20, (u64)isr20);
    set_idt_gate(21, (u64)isr21);
    set_idt_gate(22, (u64)isr22);
    set_idt_gate(23, (u64)isr23);
    set_idt_gate(24, (u64)isr24);
    set_idt_gate(25, (u64)isr25);
    set_idt_gate(26, (u64)isr26);
    set_idt_gate(27, (u64)isr27);
    set_idt_gate(28, (u64)isr28);
    set_idt_gate(29, (u64)isr29);
    set_idt_gate(30, (u64)isr30);
    set_idt_gate(31, (u64)isr31); 
    
    set_idt_gate(32, (u64)irq0);
    set_idt_gate(33, (u64)irq1);
    set_idt_gate(34, (u64)irq2);
    set_idt_gate(35, (u64)irq3);
    set_idt_gate(36, (u64)irq4);
    set_idt_gate(37, (u64)irq5);
    set_idt_gate(38, (u64)irq6);
    set_idt_gate(39, (u64)irq7);
    set_idt_gate(40, (u64)irq8);
    set_idt_gate(41, (u64)irq9);
    set_idt_gate(42, (u64)irq10);
    set_idt_gate(43, (u64)irq11);
    set_idt_gate(44, (u64)irq12);
    set_idt_gate(45, (u64)irq13);
    set_idt_gate(46, (u64)irq14);
    set_idt_gate(47, (u64)irq15);
    set_idt();
}

char *exception_messages[] = {
    "Division by zero",
    "debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

 MS_ABI void isr_handler(registers_t *r){
    print("interrupt :");
    char s[17];
    int_to_ascii(r->error_code,s);

    print(exception_messages[r->vector]);
    print(", error code:");
    print(s);
    print("\n");
    
}

MS_ABI void register_interrupt_handler(u8 n, isr_t handler){
    interrupt_handlers[n] = handler;
}

MS_ABI void irq_handler(registers_t *r){
  
    if(interrupt_handlers[r->vector]!=0){
        isr_t handler = interrupt_handlers[r->vector];
        handler(r);
    }
      lapic_eoi();

}