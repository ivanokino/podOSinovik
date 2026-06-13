

#include "../utils/types.h"
#include "../drivers/screen.h"
#include "../cpu/isr_irq.h"
#include "../drivers/APIC.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"
#include "../utils/string.h"
#include "../drivers/memory.h"
extern void init_paging(void) __attribute__((weak));
    void kernel_main(struct framebuffer_info* fb, u32 lapic_base, u32 ioapic_base, struct MemMapInfo* MemMapInfo) {

       
        u32* fb_base = (u32*)fb->base;


        g_lapic_base = lapic_base;
      
        screen_init(fb);
        clear_screen();

        isr_install();  
        init_apic(lapic_base, ioapic_base);
        init_keyboard();
        
        print("welcome to podOSinovik\n");
        
        print("\n");

        char* format[3];
        int_to_ascii(fb->pixel_format, format);
        if(strcmp(format, "1")==0)
        print("BGR\n");
        else print ("RGB\n");
        print_largest_block(MemMapInfo);

    //    for (u32 i = 0; i < fb->width * fb->height; i++) {
    //     fb_base[i] = 0x00404040;
    // }



    asm volatile("sti");    

        while(1) {       
            asm volatile("hlt");
    }  
}

