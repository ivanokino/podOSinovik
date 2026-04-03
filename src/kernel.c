#include "structs.h"
__attribute__((ms_abi))

    u32 make_need_color(u8 r, u8 g, u8 b, u8 format) {
        if (format == 0) {
            return (r << 16) | (g << 8) | b;
        } else {
            return (b << 16) | (g << 8) | r;
        }
    }

    __attribute__((ms_abi))
    void kernel_main(struct framebuffer_info* fb) {

    
        u32* fb_base = (u32*)fb->base;




    for (u32 y = 0; y < 1000; y++) {
        for (u32 x = 0; x < 1000; x++) {
            u32 index = y * fb->pitch + x;
            fb_base[index] = 0x00FF0000;
        }
    }
        
        while(1) {
            
            asm volatile("hlt");
    }  
    }