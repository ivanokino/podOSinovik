
#ifndef PORTS_H
#define PORTS_H
#include "../utils/abi.h"
MS_ABI unsigned char port_byte_get(unsigned short port);
MS_ABI void port_byte_send(unsigned short port, unsigned char data);
MS_ABI unsigned char port_short_get(unsigned short port);
MS_ABI void port_short_send(unsigned short port, unsigned short data);

#endif 


