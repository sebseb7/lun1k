#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "tetris.h"

enum {
	MAX_PLAYERS = 3
};

static Grid grids[MAX_PLAYERS];


void tetris_load() {
	int i;
	for(i = 0; i < MAX_PLAYERS; i++) init_grid(&grids[i],i);
	tetris_set_single_mode(0);	
	
}

void tetris_load_single() {
	init_grid(&grids[0],0);
	tetris_set_single_mode(1);	
}

void tetris_update() {
	int i;
	for(i = 0; i < MAX_PLAYERS; i++) {
		update_grid(&grids[i]);
		draw_grid(&grids[i]);
	}
}

int add_player() {
	int i;
	for(i = 0; i < MAX_PLAYERS; i++) {
		if(activate_grid(&grids[i])) return i;
	}

	return -1;
}
int get_lines(int nr)
{
	return grids[nr].lines;
}
void remove_player(int nr) {
	init_grid(&grids[nr], nr);
}

draw_cb drawfunc;

void setDrawCb(draw_cb draw_func)
{
	drawfunc = draw_func;
}


void pixel(int x, int y, unsigned char color)
{
	drawfunc(x,y,(int)color);
}

