#ifndef __AT24C02_H_
#define __AT24C02_H_

#include "defs.h"

#define AT24C02_WRITE_ADDR 0xA0
#define AT24C02_READ_ADDR 0xA1


void at24c02_init();
void at24c02_write_byte(u8 addr, u8 dat);
u8 at24c02_read_byte(u8 addr);

// For the following 2 functions: 
// Due to hardware restriction, addr shoud be 8Byte-aligned and max value for count is 8 
void at24c02_write(u8 addr, u8 *dat, u16 count); // count is byte count. 
void at24c02_read(u8 addr, u8 *dat, u16 count);

#endif