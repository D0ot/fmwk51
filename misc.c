#include "defs.h"
#include "misc.h"
#include <intrins.h>

void Delay3000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void delay(u16 x) {
	while(--x) {
		u8 y = 114;
		while(--y);
	}
}

void delay_5us() {
	_nop_();
}

void delay_ticks(u16 ticks) {
	while(ticks--);
}