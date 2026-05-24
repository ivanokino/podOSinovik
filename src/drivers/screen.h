 #ifndef SCREEN_H
 #define SCREEN_H

 #include "../utils/types.h"

#include "../bootloader/structs.h"
 void screen_init(struct framebuffer_info* fb);

 u32 make_need_color(u8 r, u8 g, u8 b);

 void put_pixel(u32 x, u32 y, u32 color);

 void draw_char(char c, u32 x, u32 y, u32 fg, u32 bg);

 void putchar(char c);
 
 void print(char* str);
#endif