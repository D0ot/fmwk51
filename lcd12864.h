#ifndef __LCD12864_H_
#define __LCD12864_H_

#include "defs.h"
#define LCD_DATA  P0
sbit LCD_RS  =  P3^5;
sbit LCD_RW  =  P3^6;
sbit LCD_EN  =  P3^4;
sbit LCD_PSB =  P3^1;

void lcd_init();
bit lcd_busy();
void lcd_write_cmd(u8 cmd);
void lcd_write_data(u8 dat);
void lcd_display(u8 x, u8 y, u8 *dat);
void lcd_raw_display(u8 *dat, u8 s);
#define LCD_CURSOR_ON 0x0f
#define LCD_CURSOR_OFF 0x0c


#endif