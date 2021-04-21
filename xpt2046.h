#ifndef __XPT2046_H_
#define __XPT2046_H_

#include "defs.h"

// from 0(0V) to 1048 (5V)

u16 xpt2046_read(u8);

#define AD_CH0 0x94
#define AD_CH1 0xd4
#define AD_CH2 0xa4
#define AD_CH3 0xe4 
//通道0光敏cmd：0x94  通道1热敏cmd：0xd4  通道2电位器cmd：0xa4 通道3外部输入AIN3 cmd：0xe4

#endif