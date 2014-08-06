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
#include "mcugui/text.h"
#include "libs/armmath.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static uint8_t *bzr_a, *bzr_b, *bzr_c;
static uint8_t *t_bzr_a, *t_bzr_b, *t_bzr_c;


static void bzr_init(void) {
	bzr_a = malloc(64*64 * sizeof(*bzr_a));
	bzr_b = malloc(64*64 * sizeof(*bzr_b));
	bzr_c = malloc(64*64 * sizeof(*bzr_c));
	t_bzr_a = malloc(64*64 * sizeof(*t_bzr_a));
	t_bzr_b = malloc(64*64 * sizeof(*t_bzr_b));
	t_bzr_c = malloc(64*64 * sizeof(*t_bzr_c));

	for(int y = 0, p = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++, p++) {
			bzr_a[p] = rand() & 0xFF;
			bzr_b[p] = rand() & 0xFF;
			bzr_c[p] = rand() & 0xFF;
		}
	}
}

static void bzr_deinit(void) {
	free(bzr_a);
	free(bzr_b);
	free(bzr_c);
	free(t_bzr_a);
	free(t_bzr_b);
	free(t_bzr_c);
}

static uint8_t bzr_buffer[64][64][3];

static void put_bzr_buffer(uint8_t x,uint8_t y,uint8_t r, uint8_t g, uint8_t b)
{
	bzr_buffer[x][y][0] = r;
	bzr_buffer[x][y][1] = g;
	bzr_buffer[x][y][2] = b;
}

static uint8_t bzr_tick(void) {
	static int time = 0;
	time++;
	

	for(int y = 0, p = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++, p++) {
			/* Compute neighbor averages, with wrap-around. */
			int16_t sa = 0, sb = 0, sc = 0;
			for(int j = y - 1 ; j < y + 2; j++) {
				for(int i = x - 1; i < x + 2; i++) {
					int q =
					(j < 0 ? j + 64 : j >= 64 ? j - 64 : j) * 64 +
					(i < 0 ? i + 64 : i >= 64 ? i - 64 : i);
					sa += bzr_a[q];
					sb += bzr_b[q];
					sc += bzr_c[q];
				}
			}


			sa /= 9;
			sb /= 9;
			sc /= 9;

			int16_t ta = (sa * (259 + sb - sc)) >> 8;
			int16_t tb = (sb * (259 + sc - sa)) >> 8;
			int16_t tc = (sc * (259 + sa - sb)) >> 8;
			t_bzr_a[p] = MIN(ta,255);
			t_bzr_b[p] = MIN(tb,255);
			t_bzr_c[p] = MIN(tc,255);
			put_bzr_buffer(x, y, 
					sini(time*50+50*t_bzr_a[p])>>8  ,
					sini(0x1555+time*70+30*t_bzr_a[p])>>8  ,
					sini(0x2aaa+time*30+20*t_bzr_a[p])>>8

					);
		}
	}  

	for(int y = 0, p = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++, p++) {
			bzr_a[p] = t_bzr_a[p];
			bzr_b[p] = t_bzr_b[p];
			bzr_c[p] = (t_bzr_a[p]>>4)+(t_bzr_c[p]);

		}
	}

	return 0;
}


static uint16_t **zTable;

/* No idea why the one in math.h doesn't work */
static double pythagoras1( double side1, double side2 )
{
	return sqrt(side1*side1+side2*side2);
}

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
                (pythagoras1(x1, y1)
                 + 1);
            uint16_t z = MIN(0xfff, 0xfff * distance);
            zTable[y][x] = z;
        }
    }

	bzr_init();
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
	bzr_deinit();
}


static uint32_t getTex(uint16_t a, uint16_t z) {

//	printf("%i %i\n",a,z);

	uint32_t r =  1;//bzr_buffer[(a>>6)%64][z%64][0]<<16;
	uint32_t g =  1;//bzr_buffer[(a>>6)%64][z%64][2]<<8;
	uint32_t b =  bzr_buffer[(a>>6)%64][z%64][1];

//	uint32_t r = sini((z<<8)+(a<<4))>>8<<16;
//	uint32_t g = sini(z*100)>>8<<8;
	//uint32_t b = sini(z*1600+(a>>4))>>8;
//	uint32_t b2 = sini((z<<7)+(a<<1))>>8;
			
//			h=t*sini(a*3+z*70)/512;
//			hsv_to_rgb();
//			printf(" %i %i %i %i\n",i,nr,ng,nb);


//	r |= nr>>2<<16;
//	g |= ng>>2<<8;
//	b |= nb>>2;

	return r|g|b;

	//return texGetRGB(z, a >> 6);
}


// as this is a "screensaver" you should display the message/nick somewhere

static uint8_t tick(void) {
	
	bzr_tick();

	static uint16_t t = 0;
    t+=1;
   
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
            uint32_t texel = getTex(a + t * 0x41, z + t * 1);
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
    registerAnimation("Tunnel BZ",init,tick,deinit, 0, 1000);
}




