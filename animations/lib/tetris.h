#ifndef TETRIS_H_
#define TETRIS_H_

#include "grid.h"
#include "main.h"

typedef void (*draw_cb)(int,int,int);


void setDrawCb(draw_cb);

void tetris_load(uint8_t players, int8_t human);
void tetris_update(void);

int add_player(void);
int get_lines(int nr);

#endif
