#include "../utils/structs.h"
#include "../drivers/screen.h"


    MS_ABI
    void kernel_main(struct framebuffer_info* fb) {

    
        u32* fb_base = (u32*)fb->base;

    for (u32 y = 0; y < fb->height; y++) {
        for (u32 x = 0; x < fb->width; x++) {
            u32 index = y * fb->pitch + x;
            fb_base[index] = 0x00000000;
        }
    }
        
        screen_init(fb);
        print("welcome to podOSinovik\n");
        

        while(1) {
            
            asm volatile("hlt");
    }  
    }