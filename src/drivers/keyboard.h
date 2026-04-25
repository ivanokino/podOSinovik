#ifndef KEYBOARD_H
#define KEYBOARD_H
#include"screen.h"
#include "../cpu/isr_irq.h"
#include "../utils/types.h"


MS_ABI void init_keyboard();
void print_letter(u8 scancode);  
#endif
