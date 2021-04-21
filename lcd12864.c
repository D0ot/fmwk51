#include "lcd12864.h"
#include "misc.h"
#include <intrins.h>

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

void lcd_init()
{
    LCD_PSB = 1;         //并口方式
    lcd_write_cmd(0x34);      //扩充指令操作
    delay(5);
    lcd_write_cmd(0x30);      //基本指令操作
    delay(5);
    lcd_write_cmd(0x0C);      //显示开，关光标
    delay(5);
    lcd_write_cmd(0x01);      //清除LCD的显示内容
    delay(5);
}

bit lcd_busy()
{
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(LCD_DATA&0x80);
    LCD_EN = 0;
    return(result);
}


void lcd_write_cmd(u8 cmd)
{
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    LCD_DATA = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}

void lcd_write_data(u8 dat)
{
	  while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_DATA = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}

u8 code lcd_offset_table[4] = {0x80, 0x90, 0x88, 0x98};
u8 lcd_i;
void lcd_display(u8 x, u8 y,u8* dat)
{
	x = lcd_offset_table[x];
	lcd_write_cmd(x + y);
	for(lcd_i = 0; lcd_i < 16 && dat[lcd_i]; ++lcd_i) {
		lcd_write_data(dat[lcd_i]);
	}
}

void lcd_raw_display(u8 *dat, u8 s) {
	for(lcd_i = 0; lcd_i < s && dat[lcd_i]; ++lcd_i) {
		lcd_write_data(dat[lcd_i]);
	}
}
