#ifndef __PIXEL_HAL_H
#define __PIXEL_HAL_H


void setLedXY(uint8_t x,uint8_t y, uint8_t r,uint8_t g,uint8_t b);
void getLedXY(uint8_t x, uint8_t y, uint8_t* r,uint8_t* g, uint8_t* b);
void invLedXY(uint8_t x, uint8_t y);
#endif
