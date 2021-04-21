#include "at24c02.h"
#include "i2c.h"
#include "misc.h"

void at24c02_init() {
	i2c_init();
}

void at24c02_write_byte(u8 addr, u8 dat) {
	i2c_start();
	i2c_write(AT24C02_WRITE_ADDR);
	
	i2c_write(addr);
	
	i2c_write(dat);
	
	i2c_stop();
}

u8 at24c02_read_byte(u8 addr) {
	u8 buf;
	i2c_start();
	i2c_write(AT24C02_WRITE_ADDR);
	
	i2c_write(addr);

	i2c_re_start();
	i2c_write(AT24C02_READ_ADDR);
	buf = i2c_read();
	i2c_nack();
	i2c_stop();
	return buf;
}

void at24c02_write(u8 addr, u8 *dat, u16 count) {
	i2c_start();
	i2c_write(AT24C02_WRITE_ADDR);
	i2c_write(addr);
	
	while(count--) {
		i2c_write(*dat++);
	}
	i2c_stop();
}

void at24c02_read(u8 addr, u8 *dat, u16 count){
	i2c_start();
	i2c_write(AT24C02_WRITE_ADDR);
	i2c_write(addr);
	
	i2c_re_start();
	i2c_write(AT24C02_READ_ADDR);
	while(--count) {
		*dat = i2c_read();
		dat++;
		i2c_ack();
	}
	*dat = i2c_read();
	i2c_nack();
	i2c_stop();
	i2c_stop();
}
