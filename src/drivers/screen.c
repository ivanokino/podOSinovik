
#include "font.h"

#include "screen.h"
static struct framebuffer_info* s_fb_inf = 0;
#define WHITE 0x00ffffff
#define BLACK 0x00000000
static u32 s_cursor_x = 0;
static u32 s_cursor_y = 0;
static u32 s_font_width = 8;
static u32 s_font_height = 8;
static u32 s_fg_color = 0x00ffffff;//white
static u32 s_bg_color = 0x00000000;

MS_ABI void screen_init(struct framebuffer_info* fb){

    s_fb_inf = fb;
    
}

MS_ABI u32 make_need_color(u8 r, u8 g, u8 b) {
    if (s_fb_inf->pixel_format == 0) {
        return (r << 16) | (g << 8) | b;
    } else {
        return (b << 16) | (g << 8) | r;
    }
}

MS_ABI void put_pixel(u32 x, u32 y, u32 color){
    if(x>=s_fb_inf->width || y>= s_fb_inf->height) return;
    u32 *base = (u32*)s_fb_inf->base;
    u32 index = y* s_fb_inf->pitch + x;
    base[index] = color;
}

MS_ABI void draw_char(char c, u32 x, u32 y, u32 fg, u32 bg){
    if(c<32 || c>126) return;
    const u8 *glyph = font[c-32]; 
    if(x+s_font_width > s_fb_inf->width) return;
    if(y+s_font_height > s_fb_inf->height) return;

    u32* base = (u32*)s_fb_inf->base;
    u32 pitch = s_fb_inf->pitch;

    for(u32 row=0; row <s_font_height; row++){
        u8 line = glyph[row];
        u32*line_ptr = base+(y+row)*pitch +x;   
        for(u32 col =0; col < s_font_width; col++){
            if(line &(1<<(7-col))){
            line_ptr[col] = fg;}else line_ptr[col] = bg;
        }
    }

}




MS_ABI void putchar(char c){
    if (c=='\n'){
        s_cursor_x =0;
        s_cursor_y += s_font_height;
    }
    else if (c=='\r'){
        s_cursor_x = 0;
    }else if(c=='\t'){
        for(int i=0; i<4; i++) putchar(' ');
    }else if(c=='\b'){
        if(s_cursor_x>0){
            s_cursor_x-=s_font_width;
            draw_char(' ', s_cursor_x, s_cursor_y, s_fg_color, s_bg_color);
        }
    }
    else{
        draw_char(c, s_cursor_x, s_cursor_y, s_fg_color, s_bg_color);
        s_cursor_x += s_font_width;
    }
    if(s_cursor_x + s_font_width > s_fb_inf->width){
        s_cursor_x =0; 
        s_cursor_y += s_font_height; 
    }
    if(s_cursor_y + s_font_height>s_fb_inf->height){
        s_cursor_x =0;
        s_cursor_y =0;
    }
} 


MS_ABI void print(char* str){
    while(*str){
        putchar(*str);
        str++;
    }
}
  