#ifndef __DS18B20_H_
#define __DS18B20_H_

#include "defs.h"

sbit DS = P2 ^ 2;

void ds18b20_init();
u16 ds18b20_read();
u16 ds18b20_get_fra(u16 i);

#endif