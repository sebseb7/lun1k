#include <stdlib.h>
#include "main.h"
#include <stdio.h>


#include "bd/bd_game.h"
#include "libs/mcugui/text.h"

static int keymap;
static int releasemap;
static int ackmap;

int getkey(int key)
{
	if((keymap >> key) & 1)
	{
		ackmap |= (1 << key);
		return 1;
	}
	return 0;
}

void release_upped_keys(void)
{
	for(int i = 0; i < 8;i++)
	{
		if(releasemap & (1<<i))
		{
			keymap &= ~(1 << i);
		}
	}
	releasemap = 0;
}
static void keyup(int key)
{
	if(ackmap & (1<<key))
	{
		keymap &= ~(1 << key);
	}
	else
	{
		releasemap |= (1 << key);
	}
}

static void keydown(int key)
{
	keymap = 1 << key;
	ackmap &= ~(1 << key);
}

static void getjoy(void)
{
	uint8_t joy_x = 128;
	uint8_t joy_y = 128;

	get_stick(&joy_x,&joy_y);

	if(joy_y < 70)
	{
		keydown(0);
	}
	else if(joy_y > 140)
	{
		keydown(2);
	}
	else
	{
		keyup(0);
		keyup(2);
	}

	if(joy_x < 70)
	{
		keydown(3);
	}
	else if(joy_x > 140)
	{
		keydown(1);
	}
	else
	{
		keyup(1);
		keyup(3);
	}
}



static struct bd_game_struct_t* bd_game;
static int a;

static void init(void)
{
	bd_game = bd_game_initialize(0,0);


}

static void deinit(void)
{
	free(bd_game);
}


static int rendertick;
static int x_offset=0;
static int y_offset=0;



static uint8_t tick(void) {

	getjoy();

	a++;
	if(a > 2)
	{
		a=0;
		bd_game_process(&bd_game,getkey);
		release_upped_keys();
	}

	int zoom=8;


/*	if(bd_game->Tick < BD_UNCOVER_LOOP)
	{
		for(int line=0;line < CAVE_HEIGHT;line++)
		{
			int pos = rand()%CAVE_WIDTH;
			bd_game->covered[pos][line] = 0;
			pos = rand()%CAVE_WIDTH;
			bd_game->covered[pos][line] = 0;
			pos = rand()%CAVE_WIDTH;
			bd_game->covered[pos][line] = 0;
			pos = rand()%CAVE_WIDTH;
			bd_game.covered[pos][line] = 0;
		}
	}
*/

	//cave part:

	rendertick++;
	rendertick++;
	
	if(bd_game->Player_X > 32)
	{
		x_offset = 40-16;
	}
	else if(bd_game->Player_X < 6)
	{
		x_offset = 0;
	}
	else if(bd_game->Player_X-x_offset < 6)
	{
		if(x_offset > 0)x_offset--;
	}
	else if(bd_game->Player_X-x_offset > (40-16))
	{
		if(x_offset <= (40-16))x_offset++;
	}
	if(bd_game->Player_Y > 14)
	{
		y_offset = 22-16;
	}
	else if(bd_game->Player_Y < 6)
	{
		y_offset = 0;
	}
	else if(bd_game->Player_Y-y_offset < 6)
	{
		if(y_offset > 0)y_offset--;
	}
	else if(bd_game->Player_Y-y_offset > (22-16))
	{
		if(y_offset <= (22-16))y_offset++;
	}

	for(int y = 0; y < 16 /*CAVE_HEIGHT*/; y++) 
	{
		for(int x = 0; x < 16 /*CAVE_WIDTH*/; x++) 
		{
			int field = bd_game->cavemap[x+x_offset][y+y_offset];

		/*	if(bd_game->Tick < BD_START_DELAY)
				if(field == BD_INBOX)
					if(bd_game->Tick % 20 < 10)
						field = BD_STEELWALL;


			if(bd_game->Tick < BD_UNCOVER_LOOP)
			{
				if(bd_game->covered[x][y])
				{
					field=BD_STEELWALL;
				}
			}
	*/
			int colors[3];	
			get_colors(field,rendertick,colors);
			
			for(int a = 0; a < zoom;a++)
			{
				for(int b = 0;b < zoom;b++)
				{
					if(x*zoom+a < 128)
						if(y*zoom+b < 128)
							setLedXY(x*zoom+a,y*zoom+b,colors[0],colors[1],colors[2]);
				}
			}

		}
	}

	fill_8x6(10,110, 3,0,0,0);
	if( bd_game->DiamondsRequired - bd_game->DiamondsCollected > 0)
		draw_number_8x6(10,110,bd_game->DiamondsRequired - bd_game->DiamondsCollected,3,32,200,10,200);
	
	fill_8x6(100,110, 3,0,0,0);
	if((bd_game->CaveTime - (bd_game->Tick / 8)) > 0)
		draw_number_8x6(100,110,bd_game->CaveTime - (bd_game->Tick / 8),3,32,255,255,255);


	return 0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation("Boulderdash",init,tick,deinit, 130, 30000);
}


