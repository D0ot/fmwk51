#include "i2c.h"
#include "misc.h"
void i2c_init() {
	i2c_stop();
}

void i2c_start() {
  // at this time, SDA = 1, SCL = 1
	SDA = 0;
	delay_5us();
	SCL = 0;
}

void i2c_re_start() {
	SDA = 1;
	delay_5us();
	SCL = 1;
	delay_5us();
	SDA = 0;
	delay_5us();
	SCL = 0;
}

void i2c_stop() {
	SDA = 0;
  delay_5us();
	SCL = 1;
	delay_5us(); // t_SU;STO 4us
	SDA = 1;
	delay_5us(); // t_BUF 4.7us
}

bit i2c_write(u8 b) {
	u8 i = 8;
	while(i--) {
		delay_5us();
		SDA = b & 0x80;
		// t_SU;DAT 250ns
		SCL = 1;
		delay_5us(); // t_HIGH 4us
		SCL = 0;
		b = b << 1;
	}
	return i2c_read_ack();
}

u8 i2c_read() {
	u8 i = 8;
	u8 buf = 0;
	while(i--) {
		delay_5us();
		buf = buf << 1;
		if(SDA) {
			buf = buf | 0x01;
		}
		SCL = 1;
		delay_5us();
		SCL = 0;
	}
	return buf;
}

void i2c_ack() {
	delay_5us();
	SDA = 0;
	SCL = 1;
	delay_5us();
	SCL = 0;
	SDA = 1;
}

void i2c_nack() {
	delay_5us();
	SDA = 1;
	SCL = 1;
	delay_5us();
	SCL = 0;
}

bit i2c_read_ack() {
	bit buf = 1;
  u8 i = 0;
	delay_5us();
	SDA = 1;
	SCL = 1;
	delay_5us();
  while(SDA == 1 && ++i < 255);
	buf = SDA;
	SCL = 0;
	return buf;
}
