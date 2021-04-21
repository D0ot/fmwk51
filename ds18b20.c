#include "ds18b20.h"
#include "misc.h"
#include "intrins.h"


bit ds_init()
{
	bit i;
	DS = 1;
	_nop_();
	DS = 0;
	delay_ticks(75); //��������499.45us �ҽ��������ϵ�18B20����ȫ������λ
	DS = 1; //�ͷ�����
	delay_ticks(4); //��ʱ37.95us �ȴ�18B20���ش����ź�
	i = DS;
	delay_ticks(20); //141.95us
	DS = 1;
	_nop_();
	return (i);
}


void write_byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//����Щʱ��
		DS = dat & 0x01;
		delay_ticks(10);//76.95us
		DS = 1; //�ͷ�����׼����һ������д��
		_nop_();
		dat >>= 1;
	}
}

u16 read_byte()
{
	u8 i;
	u16 ret = 0;
	for(i = 0; i < 8; ++i)
	{
		DS = 0;
		_nop_();
		DS = 1;
		_nop_();
		ret = ret | ((u8)DS << i);
		delay_ticks(10);
		DS = 1;
		_nop_();
	}
	return ret;
}

// get raw value of temperature, mul it with 0.0625 
u16 ds18b20_read()
{
	u16 i;

// config it to 9/10/11/12 bit_ mode
/*	ds_init();
	write_byte(0xcc);
	write_byte(0x4e);
	write_byte(0x7f);
	write_byte(0xf7);
	write_byte(0x7f);
	ds_init();
	write_byte(0xcc);
	write_byte(0x48);*/
	
	ds_init();
	write_byte(0xcc); // skip rom
	write_byte(0x44); // start measure
	ds_init();
	write_byte(0xcc); // skip rom
	write_byte(0xbe); // read temp registers
	delay_ticks(5);
	i = read_byte();
	i = ((u16)read_byte() << 8) | i;
	return i;
}


// fraction part of temperature
u16 ds18b20_get_fra(u16 i)
{
	return (i & 0x0f) * 625;
}

void ds18b20_init()
{
	ds_init();
	write_byte(0xcc); // skip rom
	write_byte(0x44); // start measure
}