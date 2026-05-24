#include "keyboard.h"
#include "ports.h"
#include "screen.h"
#include "../utils/string.h"
u8 com_buffer[200];
u16 com_buffer_counter = 0;
//  void print_letter(u8 scancode);
static void keyboard_callback(registers_t *regs){
    u8 scancode = port_byte_get(0x60);
   
    if(scancode&0x80) return;
    print_letter(scancode);
}
 void init_keyboard(){
    register_interrupt_handler(IRQ1, keyboard_callback);
    print("keyboard registered\n");
}
void print_letter(u8 scancode){
   
    if(scancode & 0x80) return;
    
    switch(scancode){
        case 0x2:  // 1
            print("1");
            com_buffer[com_buffer_counter] = '1';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x3:  // 2
            print("2");
            com_buffer[com_buffer_counter] = '2';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x4:  // 3
            print("3");
            com_buffer[com_buffer_counter] = '3';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x5:  // 4
            print("4");
            com_buffer[com_buffer_counter] = '4';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x6:  // 5
            print("5");
            com_buffer[com_buffer_counter] = '5';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x7:  // 6
            print("6");
            com_buffer[com_buffer_counter] = '6';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x8:  // 7
            print("7");
            com_buffer[com_buffer_counter] = '7';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x9:  // 8
            print("8");
            com_buffer[com_buffer_counter] = '8';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0xA:  // 9
            print("9");
            com_buffer[com_buffer_counter] = '9';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0xB:  // 0
            print("0");
            com_buffer[com_buffer_counter] = '0';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0xC:  // -
            print("-");
            com_buffer[com_buffer_counter] = '-';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0xD:  // =
            print("=");
            com_buffer[com_buffer_counter] = '=';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0xE:  // Backspace
            if(com_buffer_counter > 0){
                com_buffer_counter--;
                com_buffer[com_buffer_counter] = '\0';
              
            }
              print("\b");  
            break;
        case 0x10: // Q
            print("Q");
            com_buffer[com_buffer_counter] = 'Q';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x11: // W
            print("W");
            com_buffer[com_buffer_counter] = 'W';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x12: // E
            print("E");
            com_buffer[com_buffer_counter] = 'E';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x13: // R
            print("R");
            com_buffer[com_buffer_counter] = 'R';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x14: // T
            print("T");
            com_buffer[com_buffer_counter] = 'T';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x15: // Y
            print("Y");
            com_buffer[com_buffer_counter] = 'Y';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x16: // U
            print("U");
            com_buffer[com_buffer_counter] = 'U';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x17: // I
            print("I");
            com_buffer[com_buffer_counter] = 'I';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x18: // O
            print("O");
            com_buffer[com_buffer_counter] = 'O';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x19: // P
            print("P");
            com_buffer[com_buffer_counter] = 'P';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x1A: // [
            print("[");
            com_buffer[com_buffer_counter] = '[';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x1B: // ]
            print("]");
            com_buffer[com_buffer_counter] = ']';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x1C: // Enter
            print("\n");
            
            
            execute_command(com_buffer);
            
            com_buffer_counter = 0;
            for(int i =0; i<141; i++){
                com_buffer[i]=0;
            }
            
            break;
        case 0x1E: // A
            print("A");
            com_buffer[com_buffer_counter] = 'A';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x1F: // S
            print("S");
            com_buffer[com_buffer_counter] = 'S';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x20: // D
            print("D");
            com_buffer[com_buffer_counter] = 'D';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x21: // F
            print("F");
            com_buffer[com_buffer_counter] = 'F';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x22: // G
            print("G");
            com_buffer[com_buffer_counter] = 'G';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x23: // H
            print("H");
            com_buffer[com_buffer_counter] = 'H';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x24: // J
            print("J");
            com_buffer[com_buffer_counter] = 'J';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x25: // K
            print("K");
            com_buffer[com_buffer_counter] = 'K';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;  
        case 0x26: // L
            print("L");
            com_buffer[com_buffer_counter] = 'L';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x27: // ;
            print(";");
            com_buffer[com_buffer_counter] = ';';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x28: // '
            print("'");
            com_buffer[com_buffer_counter] = '\'';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x29: // `
            print("`");
            com_buffer[com_buffer_counter] = '`';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x2B: // backslash
            print("\\");
            com_buffer[com_buffer_counter] = '\\';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x2C: // Z
            print("Z");
            com_buffer[com_buffer_counter] = 'Z';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x2D: // X
            print("X");
            com_buffer[com_buffer_counter] = 'X';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x2E: // C
            print("C");
            com_buffer[com_buffer_counter] = 'C';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x2F: // V
            print("V");
            com_buffer[com_buffer_counter] = 'V';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x30: // B
            print("B");
            com_buffer[com_buffer_counter] = 'B';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x31: // N
            print("N");
            com_buffer[com_buffer_counter] = 'N';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x32: // M
            print("M");
            com_buffer[com_buffer_counter] = 'M';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x33: // ,
            print(",");
            com_buffer[com_buffer_counter] = ',';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x34: // .
            print(".");
            com_buffer[com_buffer_counter] = '.';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x35: // /
            print("/");
            com_buffer[com_buffer_counter] = '/';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        case 0x39: // Space
            print(" ");
            com_buffer[com_buffer_counter] = ' ';
            com_buffer[com_buffer_counter + 1] = '\0';
            com_buffer_counter++;
            break;
        
 
        case 0x1D: // LCtrl
        case 0x2A: // LShift
        case 0x36: // RShift
        case 0x38: // LAlt
        case 0x37: // Keypad *
        case 0x0F: // Tab
 
            break;
            
        default:
            print("?");
            break;
    }
}

void execute_command(u8 *com_buffer){

    if(strcmp(com_buffer, "WWW")==0){ 
        print("TEST COMMAND\n");
    }
 
}