#include "../utils/types.h"
#include "../drivers/screen.h"
#include "../cpu/isr_irq.h"
#include "../drivers/APIC.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"
    MS_ABI
    void kernel_main(struct framebuffer_info* fb, u32 lapic_base, u32 ioapic_base) {

       
        u32* fb_base = (u32*)fb->base;

    for (u32 y = 0; y < fb->height; y++) {
        for (u32 x = 0; x < fb->width; x++) {
            u32 index = y * fb->pitch + x;
            fb_base[index] = 0x00000000;
        }
    }
    //     for (u32 i = 0; i < fb->width * fb->height; i++) {
    //     fb_base[i] = 0x00FF0000;
    // }
        g_lapic_base = lapic_base;
       
        screen_init(fb);
        isr_install();
        init_apic(lapic_base, ioapic_base);
        init_keyboard();
        print("welcome to podOSinovik\n");
        u8 last_scancode = 0;


    asm volatile("sti");    

        while(1) {       
            asm volatile("hlt");
    }  
}

