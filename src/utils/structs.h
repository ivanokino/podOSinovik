
#include "types.h"


#ifndef STRUCTS_H
#define STRUCTS_H





struct framebuffer_info{
    u64 base;
    u64 size;
    u32 width;
    u32 height;
    u32 pitch;
    u8 bpp; //byte for pixel
    u8 pixel_format;

    } __attribute__((packed));

#endif