#include "idt.h"

idt_gate_t idt[256];
idt_register_t idt_reg;

MS_ABI u16 get_code_selector(){
    u16 selector;
    asm volatile("mov %%cs, %0":"=r"(selector));
    return selector;
}


MS_ABI void set_idt_gate(int n, u64 handler){
    idt[n].mid_offset = middle_16(handler);
    idt[n].sel = get_code_selector();
    idt[n].always0 = 0;
    idt[n].high_offset = high_32(handler);
    idt[n].low_offset = low_16(handler);
    idt[n].flags = 0x8e;
    idt[n].always0_2nd=0;
}

MS_ABI void set_idt(){
    idt_reg.base = (u64)&idt;
    idt_reg.limit  = sizeof(idt)-1;

    asm volatile("lidtq %0"::"m"(idt_reg));
}

