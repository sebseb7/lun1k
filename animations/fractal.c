#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/armmath.h"
//#include "lib/sektor.h"

#define MAXITER 150
#define RADIUS 4
#define ZOOMSPEED 0.99
static float Maxx=0.5;             // Rightmost Real point of plane to be displayed
static float Minx=-2;            // Leftmost Real point
static float Maxy=1.25;             // Uppermost Imaginary point
static float Miny=-1.25;            // Lowermost Imaginary point
static float PoIx=-0.74977879167273;
static float PoIy=0.061208886981615;
float dx=0;
float dy=0;

static int colors[15][3] = {
				{0,0,255},
				{0,139,0},
				{0,255,255},
				{255,0,0},
				{255,0,255},
				{165,42,42},
				{240,240,240},
				{190,190,190},
				{173,216,230},
				{0,255,0},
				{224,255,255},
				{255,192,203},
				{139,0,139},
				{255,255,0},
				{255,255,255}
};

static void mandel(uint8_t xin, uint8_t yin, uint8_t *r, uint8_t *g, uint8_t *b)
{
				float x=0;
				float y=0;		//converting from pixels to points
				float xnew=0;
				uint8_t k=0;
				float offsetx = xin*dx+Minx;
				float offsety = Maxy-yin*dy;
				while(k<=MAXITER)  // Each pixel loop
				{
								//The Mandelbrot Function Z=Z*Z+c into x and y parts
								xnew=x*x-y*y + offsetx;
								y=2*x*y   + offsety;
								x=xnew;
								k++;
								if ( (x*x+y*y)>RADIUS ) break;  // Break condition
				}                      // End each pixel loop

				uint8_t color = k;
				if (color>15) color=color%15;
				if (k>MAXITER) {
								*r=0;
								*g=0;
								*b=0;
				} else {
								*r=colors[color][0];
								*g=colors[color][1];
								*b=colors[color][2];
				}
}

static uint8_t tick(void) {

				dx=(Maxx-Minx)/LED_HEIGHT;
				dy=(Maxy-Miny)/LED_WIDTH;

				uint8_t x,y,r,g,b;
				uint8_t *rp = &r; 
				uint8_t *gp = &g;
				uint8_t *bp = &b;
				for(y = 0; y < LED_HEIGHT; y++) 
				{
								for(x = 0; x < LED_WIDTH; x++) 
								{
												mandel(x,y,rp,gp,bp);
												setLedXY(x,y,r,g,b);
								}
				}
				Maxx=Maxx*ZOOMSPEED+PoIx*(1-ZOOMSPEED);
				Minx=Minx*ZOOMSPEED+PoIx*(1-ZOOMSPEED);
				Maxy=Maxy*ZOOMSPEED+PoIy*(1-ZOOMSPEED);
				Miny=Miny*ZOOMSPEED+PoIy*(1-ZOOMSPEED);

				return 0;
}




static void init(void)
{
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
				registerAnimation(init,tick,deinit, 4, 1638); 
}




