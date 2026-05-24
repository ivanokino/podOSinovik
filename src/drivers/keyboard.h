#ifndef KEYBOARD_H
#define KEYBOARD_H
#include"screen.h"
#include "../cpu/isr_irq.h"
#include "../utils/types.h"

 extern u8 com_buffer[200];
 extern u16 com_buffer_counter;
 void init_keyboard();
void print_letter(u8 scancode);  
void execute_command(u8 *com_buffer);
#endif
