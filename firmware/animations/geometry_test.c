#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "../libs/wuline.h"
#include "../libs/circle.h"
#include "../libs/text.h"

static uint16_t randr(uint16_t start,uint16_t end);

static uint8_t tick(char* nick) {

	draw_filledCircle(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255));
	//draw_filledCircleSlice(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255),randr(0,360),randr(0,360));
	draw_line(randr(0,128),randr(0,128),randr(0,128),randr(0,128),randr(0,255),randr(0,255),randr(0,255));
//	draw_text(randr(0,128),randr(0,128),"hallo",randr(0,255),randr(0,255),randr(0,255));


	return 0;
}

static void init(void) ATTRIBUTES

void init(void) {
//	registerApp(start, key);
	registerAnimation(tick, 4, 450);
}

static uint16_t randr(uint16_t start,uint16_t end)
{
	return start + rand() % ( end - start );
}

