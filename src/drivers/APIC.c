
#include "APIC.h"

u32 g_lapic_base = 0;   
MS_ABI u32 read_apic(void *addr, u32 reg){
    u32 volatile *ioapic = (u32 volatile*)addr;
    ioapic[0] = (reg&0xff);
    return ioapic[4];
}

MS_ABI void write_apic(void *addr, u32 reg, u32 value){
    u32 volatile *ioapic = (u32 volatile*)addr;
    ioapic[0] = (reg&0xff);
    ioapic[4] = value;
}

MS_ABI void init_apic(u32 lapic_base, u32 ioapic_base){
    u32 low, high;
    asm volatile("rdmsr": "=a"(low), "=d"(high):"c"(0x1b));
    low |= (1<<11);
    asm volatile ("wrmsr"::"a"(low), "d"(high), "c"(0x1b));
    
    u32 *lapic = (u32*)lapic_base;
    lapic[0xf0/4] = (lapic[0xf0/4]& -0xff)|0xff|(1<<8);//spurious vector

    void *ioapic_addr = (void*)ioapic_base;
    write_apic(ioapic_addr, 0x12, 33 ); //0x10+irq*2. for irq1
    write_apic(ioapic_addr, 0x13, 0); //0x11+irq*2s


}

MS_ABI void lapic_eoi(){ //end of interrupt
   u32 *lapic = (u32*)g_lapic_base;
   lapic[0xb0/4] =0;


}