#ifndef __PWM_H_
#define __PWM_H_

#include "defs.h"

sbit PWM_4IN = P2^4;

void pwm_init();
void pwm_set(u8 v);
void pwm_enable(u8 enable);
void timer0();

#endif