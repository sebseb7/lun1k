/* Based on: http://lodev.org/cgtutor/tunnel.html */

/*
 *
 * this version uses only about 8k ram (184k less than tunnel.c)
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "libs/mcugui/text.h"
#include "libs/armmath.h"

#define MIN(a, b) (a < b ? a : b)


static uint16_t **zTable;


static void init(void) {
    int16_t x, y;
		
	zTable = malloc(LED_WIDTH* sizeof(uint16_t *));

	for(y = 0; y < LED_HEIGHT; y++) 
    {
		zTable[y] = malloc((y+1)*sizeof(uint16_t));
        for(x = 0; x < (y+1); x++) 
        {
            int16_t
                x1 = x - LED_WIDTH,
                y1 = y - LED_HEIGHT;
            double distance =
                1 /
                (pythagoras(x1, y1)
                 + 1);
            uint16_t z = MIN(0xfff, 0xfff * distance);
            zTable[y][x] = z;
        }
    }
}

static float fast_atan2( float y, float x )
{
	const float ONEQTR_PI = M_PI / 4.0;
	const float THRQTR_PI = 3.0 * M_PI / 4.0;
	float r, angle;
	float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
	if ( x < 0.0f )
	{
		r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
	}
	else
	{
		r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
	}
	angle += (0.1963f * r * r - 0.9817f) * r;
	if ( y < 0.0f )
		return( -angle );     // negate if in quad III or IV
	else
		return( angle );
}
static uint16_t getA(uint16_t x,uint16_t y)
{
            int16_t
                x1 = x - LED_WIDTH,
                y1 = y - LED_HEIGHT;
            float angle = fast_atan2(x1, y1) ;
            return 0x1fff + (angle * 0x1fff) / 3.1514f;

}
static uint16_t getZ(uint16_t x, uint16_t y)
{

    if(x >= LED_WIDTH)
        x = (LED_WIDTH - 1)-(x-(LED_WIDTH));
    if(y >= LED_HEIGHT)
        y = (LED_HEIGHT - 1)-(y-(LED_HEIGHT));
    if(y > x)
    {
        uint8_t t = x;
        x = y;
        y = t;
    }
    return zTable[x][y];
}


static void deinit(void) {

	// free 
	for(int y = 0; y < LED_HEIGHT; y++) 
    {
		free(zTable[y]);
	}
	free(zTable);
}


static uint32_t getTex(uint16_t a, uint16_t z) {

	uint32_t r = sini((z<<8)+(a<<4))>>8<<16;
	uint32_t g = sini(z*100)>>8<<8;
	//uint32_t b = sini(z*1600+(a>>4))>>8;
	uint32_t b2 = sini((z<<7)+(a<<1))>>8;
			
//			h=t*sini(a*3+z*70)/512;
//			hsv_to_rgb();
//			printf(" %i %i %i %i\n",i,nr,ng,nb);


//	r |= nr>>2<<16;
//	g |= ng>>2<<8;
//	b |= nb>>2;

	return r|g|b2;

	//return texGetRGB(z, a >> 6);
}


// as this is a "screensaver" you should display the message/nick somewhere

static uint8_t tick(void) {
    static uint16_t t = 0;
    t+=3;
   
    int16_t shiftLookX = LED_WIDTH * (uint32_t)sini(49 * t + 0x0fff) / 0xffff;
    int16_t shiftLookY = LED_HEIGHT * (uint32_t)sini(31 * t + 0x0fff) / 0xffff;
    uint16_t x, y;

    for(y = 0; y < LED_HEIGHT; y++) 
    {
        for(x = 0; x < LED_WIDTH; x++) 
        {
            /* TODO: delta speed from shiftLook[XY] */
            uint8_t x1 = x + shiftLookX;
            uint8_t y1 = y + shiftLookY;
            uint16_t z = getZ(x1,y1);
            uint16_t a = getA(x1,y1);
            uint32_t texel = getTex(a + t * 0x7, z + t * 2);
            uint8_t r = ((texel & 0xff0000) >> 16);
            uint8_t g = ((texel & 0xff00) >> 8);
            uint8_t b = (texel & 0xff);
            /* apply shade */
    
			r = sini(r*g)>>8;
			uint16_t f = 0xff - MIN(0xff, z);
            r = f * r >> 8;
            g = f * g >> 8;
            b = f * b >> 8;
            
            setLedXY(x, y, b, g, r);
        }
    }

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
    registerAnimation("Tunnel Plasma",init,tick,deinit, 0, 1000);
}




