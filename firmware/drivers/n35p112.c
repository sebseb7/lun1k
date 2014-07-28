#include "i2c.h"
//#include "libs/reverse.h"
#include "n35p112.h"


uint8_t offset_x = 0;
uint8_t offset_y = 0;

//void i2c1_write(uint8_t addr,uint8_t reg, uint8_t byte);
//int16_t i2c1_reads16(uint8_t addr,uint8_t reg);
//int32_t i2c1_reads24(uint8_t addr,uint8_t reg);
//uint16_t i2c1_readu16(uint8_t addr,uint8_t reg);
//int16_t i2c1_read16sX(uint8_t addr,uint8_t reg);

int n35p112_init(void) {

	i2c1_init();
	
	i2c1_write(0x40,0x2b,0x00);
	i2c1_write(0x40,0x2c,0x06);
	i2c1_write(0x40,0x2d,0x08);

	Delay(10);

	uint16_t syncx =0;
	uint16_t syncy =0;
	for(uint8_t i = 0; i < 16; i++)
	{
		syncx += i2c1_readu16(0x40,0x10);
		syncy += i2c1_readu16(0x40,0x11);

		Delay(20);
	}

	syncx/=16;
	syncy/=16;

	offset_x = 0;//syncx;
	offset_y = 0;//syncy;

	return 0;
}


int get_n35p112(uint8_t *x,uint8_t *y)
{
	*x = i2c1_readu8(0x40,0x11)-offset_x+128;
	*y = 255-i2c1_readu8(0x40,0x10)-offset_y+128;
	
	return 0;
}

