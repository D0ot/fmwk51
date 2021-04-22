#include <STC89C5xRC.H>
#include <intrins.h>
#include "xpt2046.h"

#define MAIN_Fosc		11059200UL

sbit CS = P3^7;
sbit DCLK = P2^1;
sbit DIN = 	P2^0;
sbit DOUT = P2^5;


void Delay_Ms(u16 ms)
{
     u16 i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}

void SPI_Write(u8 DAT)
{
	u8 i; 
	for(i=0; i<8; i++)
	{
		DCLK = 0;
		if(DAT & 0x80)
			DIN = 1;  
		else
			DIN = 0;  
		DCLK = 1;	  
		DAT <<= 1;	  
	}
}

u16 SPI_Read()
{
	u8 i; 
	u16 DAT;
	for(i=0; i<12; i++)
	{
		DAT <<= 1; 
		DCLK = 1;  
		DCLK = 0;  
		if(DOUT)
			DAT |= 0X01;
	}
	return(DAT); 
}

u16 xpt2046_read(u8 cmd)
{
	u16 DAT;
	CS = 0;
	SPI_Write(cmd);
	DCLK = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	DAT = SPI_Read();
	CS = 1;
	return(DAT);
			
}
