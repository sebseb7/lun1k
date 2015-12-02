#include <stdlib.h>
#include "main.h"
#include <stdio.h>


#include "bd/bd_game.h"

int getkey(int key)
{
	uint8_t joy_x = 128;
	uint8_t joy_y = 128;

	get_stick(&joy_x,&joy_y);

	if(joy_y < 70)
	{
		if(key==0)return 1;
	}
	else if(joy_y > 140)
	{
		if(key==2)return 1;
	}

	if(joy_x < 70)
	{
		if(key==3)return 1;
	}
	else if(joy_x > 140)
	{
		if(key==1)return 1;
	}
	return 0;
}



static struct bd_game_struct_t* bd_game;
static unsigned int* pixelbuffer;
static int a;

static void init(void)
{
	bd_game = bd_game_initialize(0,0);
	pixelbuffer = malloc(CAVE_WIDTH*(INFO_HEIGHT+CAVE_HEIGHT)*sizeof(uint32_t));


}

static void deinit(void)
{
	free(bd_game);
	free(pixelbuffer);
}


static uint8_t tick(void) {

	a++;
	if(a < 3) return 0;

	a=0;

	bd_game_process(&bd_game,getkey);
	bd_game_render(bd_game,pixelbuffer,1);

	int zoom=3;

	for(int y = 0; y < (INFO_HEIGHT+CAVE_HEIGHT); y++) 
	{
		for(int x = 0; x < CAVE_WIDTH; x++) 
		{
			int col = pixelbuffer[((y)*CAVE_WIDTH)+x];
			uint8_t red = (col&0x00ff0000)>>16;
			uint8_t green = (col&0x0000ff00)>>8;
			uint8_t blue = (col&0x000000ff)>>0;


			for(int a = 0; a < zoom;a++)
			{
				for(int b = 0;b < zoom;b++)
				{
					setLedXY(x*zoom+a,y*zoom+b,red,green,blue);
				}
			}
		}
	}



	return 0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation("Boulderdash",init,tick,deinit, 130, 3000);
}


