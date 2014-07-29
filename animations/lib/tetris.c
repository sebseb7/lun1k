#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

enum {
	MAX_PLAYERS = 8
};

static Grid grids[MAX_PLAYERS];
static uint8_t num_players = 0;

void tetris_load(uint8_t players, int8_t human) {
	int i;
	for(i = 0; i < players; i++) init_grid(&grids[i],i,i+1 == human ? 0 : 1);
	num_players = players;

}

uint8_t tetris_get_players(void)
{
	return num_players;
}

void tetris_update() {
	int i;
	for(i = 0; i < num_players; i++) {
		update_grid(&grids[i]);
		draw_grid(&grids[i]);
	}
}

int get_lines(int nr)
{
	return grids[nr].lines;
}


static draw_cb drawfunc;

void setDrawCb(draw_cb draw_func)
{
	drawfunc = draw_func;
}


void pixel(int x, int y, unsigned char color)
{
	drawfunc(x,y,(int)color);
}

