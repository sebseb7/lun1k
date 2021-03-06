#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "lib/tetris.h"
#include "libs/mcugui/text.h"
#include "flash_db.h"

static void pixeldraw(int x, int y, int color)
{
	uint8_t r[] = {0,105,255,0  ,255,0  ,255,0  ,127,0  ,127,127,0  ,0  ,0  ,255};
	uint8_t g[] = {0,0  ,0  ,0  ,0  ,255,255,255,255,255,127,255,127,127,0  ,255};
	uint8_t b[] = {0,105 ,0  ,255,255,0  ,0  ,255,127,127,255,0  ,127,0  ,127,255};

	y+=1;

	for(int i = 0;i<5;i++)
	{
		for(int j = 0;j<5;j++)
		{
			setLedXY(x*6+j,y*6+i,r[color],g[color],b[color]);
		}
	}
}

static uint16_t highscore = 0;

static void setHighscore(uint16_t lines)
{
	highscore = lines;

}


static void init(void)
{
	setDrawCb(pixeldraw);
	setHighscoreCb(setHighscore);
	flash_db_read(ADDR_TETRIS_HIGHSCORE,&highscore);
	tetris_load(1,1);
}

static void deinit(void)
{
}


static uint8_t tick(void) {


	tetris_update();

	int lines = get_lines(0);
		
	if(lines > highscore)
	{
		flash_db_write(ADDR_TETRIS_HIGHSCORE,lines);
		highscore = lines;
	}
	fill_8x6(15,5, 3,0,0,0);
	draw_number_8x6(15,5, lines, 3, ' ' ,255,255,255);
	draw_text_8x6(5, 90, "Highscore",255,255,255);
	fill_8x6(5,100, 3,0,0,0);
	draw_number_8x6(5,100, highscore, 3, '0' ,255,255,255);

	return 0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation("Tetris",init,tick,deinit, 280, 3000);
}

