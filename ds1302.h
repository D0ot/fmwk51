#ifndef __DS1302_H_
#define __DS1302_H_

#include "defs.h"

sbit TSCLK = P2 ^ 7;
sbit TIO = P2 ^ 6;
sbit TRST = P2 ^ 4;

#define DS1302_W_ADDR 0x80  //写时钟日历寄存器起始地址
#define DS1302_R_ADDR 0x81  //读时钟日历寄存器起始地址

#define DS1302_SEC 0
#define DS1302_MIN 1
#define DS1302_HOUR 2
#define DS1302_DAY 3
#define DS1302_MON 4
#define DS1302_DOW 5
#define DS1302_YEAR 6


// S, M, H, D, M, Day of Week, Y
extern u8 TimeData[7];
void Set_DS1302_Time();
void Read_DS1302_Time();


#endif
