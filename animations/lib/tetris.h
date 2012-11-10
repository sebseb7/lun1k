#ifndef TETRIS_H_
#define TETRIS_H_

typedef void (*draw_cb)(int,int,int);


void setDrawCb(draw_cb);

void tetris_load(void);
void tetris_update(void);

int add_player(void);
int get_lines(int nr);
void remove_player(int nr);

#endif
