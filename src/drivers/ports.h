
#ifndef PORTS_H
#define PORTS_H

 unsigned char port_byte_get(unsigned short port);
 void port_byte_send(unsigned short port, unsigned char data);
 unsigned char port_short_get(unsigned short port);
 void port_short_send(unsigned short port, unsigned short data);

#endif 


