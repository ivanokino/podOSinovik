 #ifndef SCREEN_H
 #define SCREEN_H
 
 #include "../utils/types.h"
#include "../utils/abi.h"
MS_ABI void screen_init(struct framebuffer_info* fb);

MS_ABI u32 make_need_color(u8 r, u8 g, u8 b);

MS_ABI void put_pixel(u32 x, u32 y, u32 color);

MS_ABI void draw_char(char c, u32 x, u32 y, u32 fg, u32 bg);

MS_ABI void putchar(char c);
 
MS_ABI void print(char* str);
 #endif