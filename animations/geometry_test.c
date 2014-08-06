#include <stdlib.h>
#include "main.h"
#include <stdio.h>

#include "mcugui/line.h"
#include "mcugui/circle.h"
#include "mcugui/text.h"
#include "mcugui/rect.h"
#include "mcugui/button.h"
#include "libs/armmath.h"


static void init(void)
{
}
static void deinit(void)
{
}

uint8_t a = 0;

static uint8_t tick(void) {
	//sleep(1);
	//lcdFillRGB(0,0,0);

	if(a==0)
	{
		a=1;
	}
	else
	{
		a=0;
	}

	draw_filledCircle(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255));
	//draw_filledCircleSlice(randr(0,128),randr(0,128),randr(10,20),randr(0,255),randr(0,255),randr(0,255),randr(0,360),randr(0,360));
	draw_line(randr(0,128),randr(0,128),randr(0,128),randr(0,128),randr(0,255),randr(0,255),randr(0,255));
//	draw_text(randr(0,128),randr(0,128),"hallo",randr(0,255),randr(0,255),randr(0,255));

	
	draw_filledRect(randr(0,128),randr(0,128),randr(10,20),randr(10,20),randr(0,255),randr(0,255),randr(0,255));
	draw_rect(randr(0,128),randr(0,128),randr(10,20),randr(10,20),randr(1,4),randr(0,255),randr(0,255),randr(0,255));
	draw_button(randr(0,128),randr(0,128),100,a,"Button",randr(0,255),randr(0,255),randr(0,255),randr(0,255),randr(0,255),randr(0,255));
	//draw_button(10,10,100,a,"Button",randr(0,255),randr(0,255),randr(0,255));
	draw_button8(randr(0,128),randr(0,128),60,a,"Button",randr(0,255),randr(0,255),randr(0,255),randr(0,255),randr(0,255),randr(0,255));
	

	return 0;
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation("Geometry Test",init,tick,deinit, 100,500);
	
}

