#ifndef APIC_H
#define APIC_H
#include "../utils/types.h"
#include "../utils/abi.h"

extern u32 g_lapic_base;

MS_ABI u32 read_apic(void *addr, u32 reg);
MS_ABI void write_apic(void* addr, u32 reg, u32 value);
MS_ABI void lapic_eoi();

MS_ABI void init_apic(u32 lapic_base, u32 ioapic_base);



#endif