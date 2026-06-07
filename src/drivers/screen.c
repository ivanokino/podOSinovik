
#include "font.h"
#include "keyboard.h"
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


 void screen_init(struct framebuffer_info* fb){

    s_fb_inf = fb;
    
}

 u32 make_need_color(u8 r, u8 g, u8 b) {
    if (s_fb_inf->pixel_format == 0) {
        return (r << 16) | (g << 8) | b;
    } else {
        return (b << 16) | (g << 8) | r;
    }
}

 void put_pixel(u32 x, u32 y, u32 color){
    if(x>=s_fb_inf->width || y>= s_fb_inf->height) return;
    u32 *base = (u32*)s_fb_inf->base;
    u32 index = y* s_fb_inf->pitch + x;
    base[index] = color;
}

 void draw_char(char c, u32 x, u32 y, u32 fg, u32 bg){
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

 void putchar(char c){

      if(s_cursor_x + s_font_width > s_fb_inf->width){
        s_cursor_x =0; 
        s_cursor_y += s_font_height; 
            com_buffer_counter = 0;
            for(int i =0; i<200; i++){
                com_buffer[i]=0;
            }
    }

    if (c=='\n'){
        s_cursor_x =0;
        s_cursor_y += s_font_height;return;
    }
    else if (c=='\r'){
        s_cursor_x = 0;return;
    }else if(c=='\t'){
        for(int i=0; i<4; i++) putchar(' ');
    }else if(c=='\b'){
        if(s_cursor_x>0){
            s_cursor_x-=s_font_width;
            draw_char(' ', s_cursor_x, s_cursor_y, s_fg_color, s_bg_color);
        }else if(s_cursor_y >0){
            s_cursor_x = s_fb_inf->width-s_font_width;
            s_cursor_y-=s_font_height;
            draw_char(' ', s_cursor_x, s_cursor_y, s_fg_color, s_bg_color);
        }
    }
    else{
        draw_char(c, s_cursor_x, s_cursor_y, s_fg_color, s_bg_color);
        s_cursor_x += s_font_width;
    }
  
    if(s_cursor_y + s_font_height>s_fb_inf->height){
        s_cursor_x =0;
        s_cursor_y =0;
    }
} 


 void print(char* str){
    while(*str){
        putchar(*str);
        str++;
    }
}
  
 void move(u32 x, u32 y){
    s_cursor_x =x; s_cursor_y =y;
 }
 
 void clear_screen(){
      for (u32 y = 0; y < s_fb_inf->height; y++) {
        for (u32 x = 0; x < s_fb_inf->width; x++) {
            u32 index = y * s_fb_inf->pitch + x;
            u32* fb_base = (u32*)s_fb_inf->base;
            fb_base[index] = 0x0000000;
        }
    } 
    move(0,0);
 }  



void draw_image(int x, int y, int w, int h, const u32 *pixels) {
   
  u32* fb_base = (u32*)s_fb_inf->base;
        for(int row =0; row <h &&  (y+row)< s_fb_inf->height;row++ ){
            for(int col =0; col < w && (x+col)<s_fb_inf->width;col++){
                int src_idx = row* w+col;
                int dst_idx = (y+row)*s_fb_inf->pitch +(x+col);
                fb_base[dst_idx] = pixels[src_idx];
            }
        }
}