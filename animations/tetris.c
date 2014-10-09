#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "lib/tetris.h"
#include "libs/mcugui/text.h"

static void pixeldraw(int x, int y, int color)
{
	uint8_t r[] = {0,100,255,0  ,255,0  ,255,0  ,127,0  ,127,127,0  ,0  ,0  ,255};
	uint8_t g[] = {0,0  ,0  ,0  ,0  ,255,255,255,255,255,127,255,127,127,0  ,255};
	uint8_t b[] = {0,100 ,0  ,255,255,0  ,0  ,255,127,127,255,0  ,127,0  ,127,255};

	x+=3;
	y+=3;

	setLedXY(x*3,y*3,r[color],g[color],b[color]);
	setLedXY(x*3+1,y*3+1,r[color],g[color],b[color]);
	setLedXY(x*3,y*3+1,r[color],g[color],b[color]);
	setLedXY(x*3+1,y*3,r[color],g[color],b[color]);

}


static void init(void)
{
	setDrawCb(pixeldraw);
	tetris_load(3,-1);
}

static void deinit(void)
{
}


static uint8_t tick(void) {


	tetris_update();

	for(int i = 0; i < 3; i++)
	{
		int lines = get_lines(i);


		fill_8x6(i * 36 +12,4, 3,0,0,0);
		draw_number_8x6(i * 36 + 12,4, lines, 3, ' ' ,255,255,255);
	}

	return 0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation("Bot Tetris",init,tick,deinit, 0, 3000);
}

