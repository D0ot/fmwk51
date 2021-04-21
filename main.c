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


u16 counter;

u8 line_buf[16];
u8 kb_event;
u8 kb_press;
u16 temperature;

volatile u16 max_t;
bit max_t_write;
u8 max_cnt;
volatile u16 min_t;
bit min_t_write;
u16 temp_save;
u8 min_cnt;

sbit LED = P3 ^ 0;

u8 temp;
u8 temp2;
u8 pwm_current;
u16 temp16;
bit stop;
u8 mode;

// global state
// 0 == default
// 1 == show max temperature
// 2 == show min temperature
// 3 == auto_
// 4 == manual
// 5 == stop

void counter_init()
{
	EA = 1;
	EX0 = 1; // enable ext interrupt_
	IT0 = 1;//  enable global interrupt_
}

void counter_interrupt() interrupt 0
{
	counter++;
}

// 50ms
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
u16 seconds;
u16 rps;

#define MAX_ADDR (16)
#define MIN_ADDR (8)

void timer1_interrupt() interrupt 3
{

	TH1 = 0x4c;
	++timer1_loop_val;
	if(timer1_loop_val == 20)
	{
		rps = counter;
		counter = 0;
		// 1 second interrupt_
		timer1_loop_val = 0;
		temperature = ds18b20_read();
		if(max_t < temperature && !stop) {
			max_t = temperature;
      max_t_write = 1;
		}
		if(min_t > temperature && !stop) {
			min_t = temperature;
      min_t_write = 1;
		}
	}
}

u8 show;
u8 xdata xc1[] = "  XC2018217758  ";
u8 xdata xc2[] = "  XC2018217691  ";
u8 code blank_line[] = "                ";
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


u8 code main_menu[] = "MAIN MENU";
u8 code auto_str[] = "AUTO";
u8 code manual_str[] = "MANUAL";
float tempf;
void main() {
	mode = 0;
	LED = 0;
  max_t = 0;
  max_t_write = 0;
  min_t = 0xffff;
  min_t_write = 0;

  
	at24c02_init();
  delay(10);
  at24c02_read(MAX_ADDR, (u8*)(&temp16), sizeof(temp16));
  max_t = temp16;
  at24c02_read(MIN_ADDR, (u8*)(&temp16), sizeof(temp16));
  min_t = temp16;
  
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
	pwm_current = 40;
	pwm_enable(1);

	while(1) {
		kb_event = kb_get_event();
		kb_press = kb_scan();
		tempf = xpt2046_read(AD_CH2) * 0.00477;

		temp16 = rps << 4;
		sprintf(line_buf, "PWM:%2bu%%", pwm_current);
		lcd_display(2, 0, line_buf);
		sprintf(line_buf, "RPM:%4u", temp16);
		lcd_display(2, 4, line_buf);

		if(mode != 2 ) {
			sprintf(line_buf, "TEMP:%u.%u", temperature >> 4, ds18b20_get_fra(temperature));
		}else {
			sprintf(line_buf, "VOLTAGE:%4.2f", tempf);
		}
		lcd_display(1, 0, line_buf);
		line_buf[0] = 0;

		if(!stop && mode !=1) {
				if(kb_event == 1 && !stop ) {
				pwm_current += 5;
				if(pwm_current > 90) {
					pwm_current = 90;
				}
				pwm_set(pwm_current);
			} else if(kb_event == 2 && !stop) {
				pwm_current -= 5;
				if(pwm_current < 10) {
					pwm_current = 10;
				}
				pwm_set(pwm_current);
			}
		}

		if(mode == 1 && !stop) {
			pwm_current = temperature / 3 - 95;
			pwm_set(pwm_current);
		}

		if(mode == 2 && !stop) {

			if(tempf <= 2) {
				pwm_current = (u8)(tempf * 20);
			}else if(tempf < 3.5) {
				pwm_current = (u8)(tempf * 36.6) - 33;
			}else {
				pwm_current = 95;
			}
			pwm_set(pwm_current);
		}

		if(kb_event == 5) {
			if(stop) {
				pwm_current = 40;
				stop = 0;
				LED = 0;

			} else {
				pwm_current = 0;
				stop = 1;
				LED = 1;
			}
			pwm_set(pwm_current);
		} else if(kb_event == 3) {
			if(show == 1) {
				show = 0;
			}else {
				show = 1;
			}
			lcd_display(3, 0, blank_line);
		} else if(kb_event == 4) {
			if(show == 2) {
				show = 0;
			} else {
				show = 2;
			}
			lcd_display(3, 0, blank_line);
		} else if(kb_event == 7) {
			if(mode == 1) {
				mode = 0;
			}else if(mode == 0) {
				mode = 1;
			}
			lcd_display(0, 0, blank_line);
		} else if(kb_event == 8) {
			if(mode == 2) {
				mode = 0;
			} else if(mode == 0) {
				mode = 2;
			}
			lcd_display(0, 0, blank_line);
		} else if(kb_event == 6) {
      min_t = 0xffff;
      max_t = 0;
      lcd_display(3, 0, blank_line);
    }

		switch(show) {
			case 1:
				sprintf(line_buf, "MAX T:%u.%u", max_t >> 4, ds18b20_get_fra(max_t));
				lcd_display(3, 0, line_buf);
				break;
			case 2:
				sprintf(line_buf, "MIN T:%u.%u", min_t >> 4, ds18b20_get_fra(min_t));
				lcd_display(3, 0, line_buf);
				break;
		}

	  switch(mode) {
			// main menu
			case 0:
				lcd_display(0, 0, main_menu);
				break;
			// auto_mode
			case 1:
				lcd_display(0, 0, auto_str);
				break;
			// manual
			case 2:
				lcd_display(0, 0, manual_str);
				break;
		}

    if(min_t_write) {
      min_t_write = 0;
      temp_save = min_t;
      min_cnt++;
      //sprintf(line_buf, "min cnt: %bu", min_cnt);
      //lcd_display(0, 0, line_buf);
      at24c02_init();
      delay(10);
      at24c02_write(MIN_ADDR, (void*)&(temp_save), sizeof(temp_save));
    }


    if(max_t_write) {
      max_t_write = 0;
      temp_save = max_t;
      max_cnt++;
      //sprintf(line_buf, "max cnt: %bu", max_cnt);
      //lcd_display(0, 0, line_buf);
      at24c02_init();
      delay(10);
      at24c02_write(MAX_ADDR, (void*)&(temp_save), sizeof(temp_save));
    }

   
	}
}
