#ifndef __WEIGHT_H_
#define __WEIGHT_H_
#include "defs.h"

sbit ADDO = P2 ^ 7;
sbit ADSK = P2 ^ 6;

u32 readCount();
float readGram(u32 v);


#endif
