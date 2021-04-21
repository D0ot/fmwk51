#ifndef __I2C_H_
#define __I2C_H_

#include "defs.h"

sbit SDA = P2 ^ 0;
sbit SCL = P2 ^ 1;

// inactivation state of SDA and SCL are low.

void i2c_init();

void i2c_start();
void i2c_re_start();
void i2c_stop();

bit i2c_write(u8 byte);
u8 i2c_read();

void i2c_ack();
void i2c_nack();
bit i2c_read_ack();

#endif