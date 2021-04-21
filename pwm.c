#include "pwm.h"

u8 pwm_motor_val;
u8 pwm_t;


void pwm_init() {
	TMOD |= 0x02;
	TH0 = 0;
	TL0 = 220;
	TR0 = 220;
	ET0 = 1;
	EA = 1;
}

void pwm_set(u8 v) {
	if(v == 100) {
		TR0 = 0;
		PWM_4IN = 1;
	}else if(v == 0) {
		TR0 = 0;
		PWM_4IN = 0;
	} else {
		TR0 = 1;
	}
	pwm_motor_val = v / 100.0 * 255.0;
}

void pwm_enable(u8 enable) {
	TR0 = enable;
}

void timer0() interrupt 1{
	pwm_t++;
	if(pwm_t == pwm_motor_val)
		PWM_4IN = 0;
	if(pwm_t == 255)
		PWM_4IN = 1; 
}