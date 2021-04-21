#include <STC89C5xRC.H>
#include <stdio.h>
#include <intrins.h>
#include "defs.h"
#include "misc.h"
#include "lcd12864.h"
#include "keyboard.h"
#include "at24c02.h"
#include "xpt2046.h"
#include "pwm.h"
#include "ds18b20.h"
#include "at24c02.h"
#include "xpt2046.h"


void counter_init()
{
	EA = 1;
	EX0 = 1; // enable ext interrupt_
	IT0 = 1;//  enable global interrupt_
}

u16 counter;
void counter_interrupt() interrupt 0
{
	counter++;
}

// 50ms at 12T
void timer1_init()
{
	AUXR &= 0xBF;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TL1 = 0x00;
	TH1 = 0x4C;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1;
}


u8 timer1_loop_val;
void timer1_interrupt() interrupt 3
{

	TH1 = 0x4c;
	++timer1_loop_val;
	if(timer1_loop_val == 20)
	{
		timer1_loop_val = 0;
	}
}

u8 xdata xc1[] = "  XC2018217758  ";
u8 xdata xc2[] = "  XC2018217691  ";
u8 code blank_line[] = "                ";
u8 temp, temp2;
void scroll_show() {
	temp = 0;
	while(temp < 15)
	{
		lcd_display(1, 0, xc1 + temp);
		lcd_display(2, 0, "");
		temp2 = 0;
		while(temp2 < temp){
			lcd_write_data(' ');
			temp2++;
		}
		lcd_raw_display(xc2, sizeof(xc2) - temp2 - 1);
		delay(100);
		temp++;
	}
}

u16 tempf;
u8 kb_event;
void main() {
  
	at24c02_init();
  
	ds18b20_init();
	counter_init();


	lcd_init();
	lcd_display(1, 0, xc1);
	lcd_display(2, 0, xc2);
	Delay3000ms();

	scroll_show();
	timer1_init();
	pwm_init();
	pwm_set(40);
	pwm_enable(0);

	while(1) {
		kb_event = kb_get_event();
		tempf = xpt2046_read(AD_CH2) * 0.00477;
	}
}
