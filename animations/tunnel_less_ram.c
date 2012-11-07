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

#define MIN(a, b) (a < b ? a : b)


static uint8_t **zTable;
static int16_t shiftLookX = 0; //LED_WIDTH / 2;
static int16_t shiftLookY = 0; //LED_HEIGHT / 2;

static void init(void) {
    int16_t x, y;
		
	zTable = malloc(LED_WIDTH* sizeof(uint8_t *));

	for(y = 0; y < LED_HEIGHT; y++) 
    {
		zTable[y] = malloc((y+1)*sizeof(uint8_t));
        for(x = 0; x < (y+1); x++) 
        {
            int16_t
                x1 = x - LED_WIDTH,
                y1 = y - LED_HEIGHT;
            double distance =
                1 /
                (pythagoras(x1, y1)
                 + 1);
            uint8_t z = MIN(0xfff, 0xfff * distance);
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

static void deinit(void) {

	// free 
	for(int y = 0; y < LED_HEIGHT; y++) 
    {
		free(zTable[y]);
	}
	free(zTable);
}


struct brick {
    uint8_t r, g, b;
};
static struct brick bricks[] = {
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 },
    { 255, 255, 0 },
    { 0, 255, 255 },
    { 255, 255, 255}
};

static uint32_t __attribute__((always_inline)) getTex(uint16_t a, uint16_t z) {
    /* Grid, turn these knobs for texture flickr */
    uint8_t am8 = (a >> 7) & 0xf;
    uint8_t zm8 = (z >> 0) & 0x1f;
    if (am8 == 2 && zm8 == 2)
        return 0xffff7f;
    if (am8 == 2 || zm8 == 2)
        return 0x3f1f00;

    /* TODO: nifty texture */

    /* Bricks */
    uint8_t brickI = ((a >> 12) + (z >> 4)) % sizeof(bricks);
    /* printf("Brick %i/%i\t(a=%04X\tz=%04X)\n", brickI, sizeof(bricks), a, z); */
    struct brick brick = bricks[brickI];
    return (uint16_t)brick.r << 16 | (uint16_t)brick.g << 8 | (uint16_t)brick.b;
}


// as this is a "screensaver" you should display the message/nick somewhere

static uint8_t tick(void) {
    static uint16_t t = 0;
    t++;
    shiftLookX = LED_WIDTH * (uint32_t)sini(17 * t + 0x0fff) / 0xffff;
    shiftLookY = LED_HEIGHT * (uint32_t)sini(11 * t + 0x0fff) / 0xffff;
	
    uint16_t x, y;

    for(y = 0; y < LED_HEIGHT; y++) 
    {
        for(x = 0; x < LED_WIDTH; x++) 
        {
            /* TODO: delta speed from shiftLook[XY] */
			uint8_t x1 = x + shiftLookX;
			uint8_t y1 = y + shiftLookY;

			if(x1 > 127)
				x1 = 127-(x1-128);
			if(y1 > 127)
				y1 = 127-(y1-128);
			if(y1 > x1)
			{
				uint8_t t = x1;
				x1 = y1;
				y1 = t;
			}

			uint16_t z = zTable[x1][y1] + t;
            uint16_t a = getA(x + shiftLookX,y + shiftLookY) + t * 0xf;
            /* printf("%ix%i\ta=%04X\tz=%04X\n", x, y, a, z); */
            uint32_t texel = getTex(a, z);
            /* TODO: shade proportional to z */
            uint8_t r = ((texel & 0xff0000) >> 16);
            uint8_t g = ((texel & 0xff00) >> 8);
            uint8_t b = (texel & 0xff);
            setLedXY(x, y, r, g, b);
        }
    }

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
    registerAnimation(init,tick,deinit, 4, 1000);
}




