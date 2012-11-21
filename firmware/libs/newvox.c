/*
   Landscape rendering
  
   Ok.. i know that voxel is something else... but a lot of people is using
   the name "voxel" to mean this kind of rendering tecnique.
   I wrote this to explain the basic idea behind the rendering of newvox4;
   newvox4 is very badly written (it's named 4 because is the fourth of
   a sequel of experiments) and is coded in pascal + asm.
   Since i got a few request of an explanation i decided to write the kernel
   of the rendering in C hoping that this will be easier to understand.
   This implements only the base landscape (no sky or floating ball) and
   with keyboard only support but i think you can get the idea of how I
   implemented those other things.
  
   I'm releasing this code to the public domain for free... and as it's
   probably really obvious there's no warranty of any kind on it.
   You can do whatever you want with this source; however a credit in any
   program that uses part of this code would be really appreciated :)
  
   Any comment is welcome :)
  
                                    Andrea "6502" Griffini, programmer
                                           agriff@ix.netcom.com
                                        http://vv.val.net/~agriffini
*/  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "main.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef unsigned char byte;

byte *HMap = NULL;      /* Height field */
byte *CMap = NULL;      /* Color map */

/* Reduces a value to 0..255 (used in height field computation) */
static int Clamp(int x)
{
  return (x<0 ? 0 : (x>255 ? 255 : x));
}

/* Heightfield and colormap computation */
void ComputeMap(void)
{
  int p,i,j,k,k2,p2;
  FreeMap();
  HMap = malloc(256 * 256);
  CMap = malloc(256 * 256);

  /* Start from a plasma clouds fractal */
  HMap[0]=128;
  for ( p=256; p>1; p=p2 )
  {
    p2=p>>1;
    k=p*8+20; k2=k>>1;
    for ( i=0; i<256; i+=p )
    {
      for ( j=0; j<256; j+=p )
      {
	int a,b,c,d;

	a=HMap[(i<<8)+j];
	b=HMap[(((i+p)&255)<<8)+j];
	c=HMap[(i<<8)+((j+p)&255)];
	d=HMap[(((i+p)&255)<<8)+((j+p)&255)];

	HMap[(i<<8)+((j+p2)&255)]=
	  Clamp(((a+c)>>1)+(rand()%k-k2));
	HMap[(((i+p2)&255)<<8)+((j+p2)&255)]=
	  Clamp(((a+b+c+d)>>2)+(rand()%k-k2));
	HMap[(((i+p2)&255)<<8)+j]=
	  Clamp(((a+b)>>1)+(rand()%k-k2));
      }
    }
  }

  /* Smoothing */
  for ( k=0; k<3; k++ )
    for ( i=0; i<256*256; i+=256 )
      for ( j=0; j<256; j++ )
      {
	HMap[i+j]=(HMap[((i+256)&0xFF00)+j]+HMap[i+((j+1)&0xFF)]+
		   HMap[((i-256)&0xFF00)+j]+HMap[i+((j-1)&0xFF)])>>2;
      }

  /* Color computation (derivative of the height field) */
  for ( i=0; i<256*256; i+=256 )
    for ( j=0; j<256; j++ )
    {
      k=192+(HMap[((i+256)&0xFF00)+((j+1)&255)]-HMap[i+j])*8;
      if ( k<1 ) k=1; if (k>255) k=255;
      CMap[i+j]=k;
    }
}

void FreeMap() {
  if (HMap)
  {
    free(HMap);
    HMap = NULL;
  }
  if (CMap)
  {
    free(CMap);
    CMap = NULL;
  }
}

int lasty[LED_WIDTH],         /* Last pixel drawn on a given column */
    lastc[LED_WIDTH];         /* Color of last pixel on a column */

/*
   Draw a "section" of the landscape; x0,y0 and x1,y1 and the xy coordinates
   on the height field, hy is the viewpoint height, s is the scaling factor
   for the distance. x0,y0,x1,y1 are 16.16 fixed point numbers and the
   scaling factor is a 16.8 fixed point value.
 */
static void Line(int x0,int y0,int x1,int y1,int hy,int s)
{
  int i,sx,sy;

  /* Compute xy speed */
  sx=(x1-x0)/LED_WIDTH; sy=(y1-y0)/LED_WIDTH;
  for ( i=0; i<LED_WIDTH; i++ )
  {
    int c,y,h,u0,v0,u1,v1,a,b,h0,h1,h2,h3;

    /* Compute the xy coordinates; a and b will be the position inside the
       single map cell (0..255).
     */
    u0=(x0>>16)&0xFF;    a=(x0>>8)&255;
    v0=((y0>>8)&0xFF00); b=(y0>>8)&255;
    u1=(u0+1)&0xFF;
    v1=(v0+256)&0xFF00;

    /* Fetch the height at the four corners of the square the point is in */
    h0=HMap[u0+v0]; h2=HMap[u0+v1];
    h1=HMap[u1+v0]; h3=HMap[u1+v1];

    /* Compute the height using bilinear interpolation */
    h0=(h0<<8)+a*(h1-h0);
    h2=(h2<<8)+a*(h3-h2);
    h=((h0<<8)+b*(h2-h0))>>16;

    /* Fetch the color at the four corners of the square the point is in */
    h0=CMap[u0+v0]; h2=CMap[u0+v1];
    h1=CMap[u1+v0]; h3=CMap[u1+v1];

    /* Compute the color using bilinear interpolation (in 16.16) */
    h0=(h0<<8)+a*(h1-h0);
    h2=(h2<<8)+a*(h3-h2);
    c=((h0<<8)+b*(h2-h0));

    /* Compute screen height using the scaling factor */
    y=(((h-hy)*s)>>11)+100;

    /* Draw the column */
    if ( y<(a=lasty[i]) )
    {
      int sc,cc;


      if ( lastc[i]==-1 )
	lastc[i]=c;

      sc=(c-lastc[i])/(a-y);
      cc=lastc[i];

      if ( a>=LED_HEIGHT ) { b-=(a+1-LED_HEIGHT)*LED_WIDTH; cc+=(a+1-LED_HEIGHT)*sc; a=LED_HEIGHT-1; }
      if ( y<0 ) y=0;
      while ( y<a )
      {
        if (h < 23) {
          /* Snow tops */
          uint8_t w = 0xff - (cc >> 20);
          setLedXY(i, a, w, w, w);
        } else if (h > 0xe5)
          /* Rivers & Lakes */
          setLedXY(i, a, 0, 0, 0x7f - (cc >> 18));
        else
          /* Grass */
          setLedXY(i, a, cc >> 19, MIN(0xff, (cc >> 19) + 0x7f), cc >> 19);
	cc+=sc;
	a--;
      }
      lasty[i]=y;

    }
    lastc[i]=c;

    /* Advance to next xy position */
    x0+=sx; y0+=sy;
  }
}

float FOV=3.141592654/4;   /* half of the xy field of view */

/*
// Draw the view from the point x0,y0 (16.16) looking at angle a
*/
void View(int x0,int y0,float aa)
{
  int d;
  int a,b,h,u0,v0,u1,v1,h0,h1,h2,h3;

  /* Draw horizon */
  for(int y = 0; y < 128; y++)
    for(int x = 0; x < 128; x++) {
      setLedXY(x,y, y, y, 127);
    }
  /* draw_filledCircle(LED_WIDTH / 2, LED_HEIGHT / 4, 32, */
  /*                   0xff, 0xff, 0x7f); */
  /* Initialize last-y and last-color arrays */
  for ( d=0; d<LED_WIDTH; d++ )
  {
    lasty[d]=LED_HEIGHT;
    lastc[d]=-1;
  }

  /* Compute viewpoint height value */

  /* Compute the xy coordinates; a and b will be the position inside the
     single map cell (0..255).
   */
  u0=(x0>>16)&0xFF;    a=(x0>>8)&255;
  v0=((y0>>8)&0xFF00); b=(y0>>8)&255;
  u1=(u0+1)&0xFF;
  v1=(v0+256)&0xFF00;

  /* Fetch the height at the four corners of the square the point is in */
  h0=HMap[u0+v0]; h2=HMap[u0+v1];
  h1=HMap[u1+v0]; h3=HMap[u1+v1];

  /* Compute the height using bilinear interpolation */
  h0=(h0<<8)+a*(h1-h0);
  h2=(h2<<8)+a*(h3-h2);
  h=((h0<<8)+b*(h2-h0))>>16;

  /* Draw the landscape from near to far without overdraw */
  for ( d=0; d<100; d+=1+(d>>6) )
  {
    Line(x0+d*0x7fff*cos(aa-FOV),y0+d*0x7fff*sin(aa-FOV),
         x0+d*0x7fff*cos(aa+FOV),y0+d*0x7fff*sin(aa+FOV),
         h-30,150*256/(d+1));
  }
}
