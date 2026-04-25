#ifndef IDT_H
#define IDT_H

#include "../utils/types.h"
#include "../utils/abi.h"



typedef struct {
    u16 low_offset;
    u16 sel;
    u8 always0; //interrupt stack table IST
    u8 flags;
    u16 mid_offset;
    u32 high_offset;
    u32 always0_2nd;
}__attribute__((packed)) idt_gate_t;

typedef struct{
    u16 limit;
    u64 base;
}__attribute__((packed)) idt_register_t;

extern idt_gate_t idt[256];
extern idt_register_t idt_reg;

MS_ABI void set_idt_gate(int n, u64 handler);
MS_ABI void set_idt();









#endif