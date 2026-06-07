
   #include "APIC.h"

   u32 g_lapic_base=0;
   u32 read_ioapic(void *addr, u32 reg){
      u32 volatile *ioapic = (u32 volatile*)addr;
      ioapic[0] = (reg&0xff);
      return ioapic[4];
   }

   void write_ioapic(void *addr, u32 reg, u32 value){
      u32 volatile *ioapic = (u32 volatile*)addr;
      ioapic[0] = (reg&0xff);
      ioapic[4] = value;
   }
   //writing in MSR reg's 11nd byte. 
   void init_apic(u32 lapic_base, u32 ioapic_base){
      u32 low, high;
      asm volatile("rdmsr": "=a"(low), "=d"(high):"c"(0x1b));
      low |= (1<<11);
      asm volatile ("wrmsr"::"a"(low), "d"(high), "c"(0x1b));
      
      u32 *lapic = (u32*)lapic_base;
      lapic[240/4] = (lapic[240/4] & -0xff) | 0xff | (1<<8);//spurious vector.
      //example lapic[240/4] = 0b00010010 00110100 01010111 11111111;
      //                                TRASH               ||||||||                              
      //                               (but not of CPU)     0-7 - 255 idt
      //                                                  | 8 - LAPIC IS ON

      void *ioapic_addr = (void*)ioapic_base;
      write_ioapic(ioapic_addr, 0x12, 33 ); //0x10+irq*2. for irq1
      write_ioapic(ioapic_addr, 0x13, 0); //0x11+irq*2s


   }

   void lapic_eoi(){ //end of interrupt
      u32 *lapic = (u32*)g_lapic_base;
      lapic[176/4] =0;


   }