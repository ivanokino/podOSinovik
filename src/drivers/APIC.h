#ifndef APIC_H
#define APIC_H
#include "../utils/types.h"

extern u32 g_lapic_base;

 u32 read_apic(void *addr, u32 reg);
 void write_apic(void* addr, u32 reg, u32 value);
 void lapic_eoi();

 void init_apic(u32 lapic_base, u32 ioapic_base);



#endif