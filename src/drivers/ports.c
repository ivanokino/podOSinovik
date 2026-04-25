#include"ports.h"

MS_ABI unsigned char port_byte_get(unsigned short port){
    unsigned char res;
    asm("in %%dx, %%al":"=a"(res):"d"(port));
    return res;
}

MS_ABI void port_byte_send(unsigned short port, unsigned char data){
    asm("out %%al, %%dx":: "a"(data), "d"(port));
}

MS_ABI unsigned char port_short_get(unsigned short port){
    unsigned short res;
    asm("in %%dx, %%al":"=a"(res):"d"(port));
    return res;
}

MS_ABI void port_short_send(unsigned short port, unsigned short data){
    asm("out %%al, %%dx":: "a"(data), "d"(port));
}

