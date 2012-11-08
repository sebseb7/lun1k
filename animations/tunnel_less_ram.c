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
#include "libs/text.h"
#include "libs/math.h"

#include "lib/tunnel_tex.h"

#define MIN(a, b) (a < b ? a : b)


static uint16_t **zTable;

/* No idea why the one in math.h doesn't work */
static double pythagoras1( double side1, double side2 )
{
	return sqrt(side1*side1+side2*side2);
}

static void init(void) {
    int16_t x, y;
		
    zTable = malloc(2 * LED_HEIGHT * sizeof(uint16_t *));

    for(y = 0; y < 2 * LED_HEIGHT; y++) 
    {
        zTable[y] = malloc((y+1)*sizeof(uint16_t));
        for(x = 0; x < (y+1); x++) 
        {
            int16_t
                x1 = x - LED_WIDTH,
                y1 = y - LED_HEIGHT;
            double distance =
                1 /
                (pythagoras1(x1, y1)
                 + 1);
            uint16_t z = MIN(0xfff, 0xfff * distance);
            zTable[y][x] = z;
        }
    }
}

static uint16_t getA(uint16_t x,uint16_t y)
{
            int16_t
                x1 = x - LED_WIDTH,
                y1 = y - LED_HEIGHT;
            float angle = atan2f(x1, y1);
            return 0x1fff + (angle * 0x1fff) / 3.1514f;

}

static uint16_t getZ(uint16_t x, uint16_t y)
{

    if(x >= 2 * LED_WIDTH)
        x = (2 * LED_WIDTH - 1)-(x-(2 * LED_WIDTH));
    if(y >= 2 * LED_HEIGHT)
        y = (2 * LED_HEIGHT - 1)-(y-(2 * LED_HEIGHT));
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
	for(int y = 0; y < 2 * LED_HEIGHT; y++) 
    {
		free(zTable[y]);
	}
	free(zTable);
}


static uint32_t __attribute__((always_inline)) getTex(uint16_t a, uint16_t z) {
    return texGetRGB(z, a >> 6);
}


// as this is a "screensaver" you should display the message/nick somewhere

static uint8_t tick(void) {
    static uint16_t t = 0;
    t++;
    int16_t shiftLookX = LED_WIDTH * (uint32_t)sini(17 * t + 0x0fff) / 0xffff;
    int16_t shiftLookY = LED_HEIGHT * (uint32_t)sini(11 * t + 0x0fff) / 0xffff;
	
    uint16_t x, y;

    for(y = 0; y < LED_HEIGHT; y++) 
    {
        for(x = 0; x < LED_WIDTH; x++) 
        {
            /* TODO: delta speed from shiftLook[XY] */
            uint8_t x1 = x + shiftLookX;
            uint8_t y1 = y + shiftLookY;
            
            uint16_t r = 0, g = 0, b = 0;
            uint16_t z;
            for(uint8_t i = 0; i < 4; i++) {
                uint8_t x2 = x1 + (i & 1);
                uint8_t y2 = y1 + (i >> 1);
                z = getZ(x2, y2);
                uint16_t a = getA(x2,y1);
                uint32_t texel = getTex(a + t * 0x7, z + t);
                r += ((texel & 0xff0000) >> 16);
                g += ((texel & 0xff00) >> 8);
                b += (texel & 0xff);
            }
            /* printf("%ix%i\ta=%04X\tz=%04X\n", x, y, a, z); */
            /* apply shade */
            uint16_t f = 0xff - MIN(0xff, z);
            r = f * r >> 10;
            g = f * g >> 10;
            b = f * b >> 10;
            
            setLedXY(x, y, r, g, b);
        }
    }

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
    registerAnimation(init,tick,deinit, 4, 1000);
}




