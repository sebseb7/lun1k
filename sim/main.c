#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include "SDL_image.h"

#include "main.h"


#include <string.h>

#include<sys/time.h>



int sdlpause = 0;

/*unsigned long long int get_clock(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long long int)tv.tv_usec + 1000000*tv.tv_sec;
}
*/
#define MAX_ANIMATIONS 200

int animationcount = 0;

init_fun init_fp[MAX_ANIMATIONS];
tick_fun tick_fp[MAX_ANIMATIONS];
deinit_fun deinit_fp[MAX_ANIMATIONS];
int duration[MAX_ANIMATIONS];


SDL_Surface* screen;

uint8_t leds[LED_HEIGHT][LED_WIDTH][4];
void setLedXY(uint8_t x, uint8_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
	leds[y][x][3] = 1;
}

void invLedXY(uint8_t x, uint8_t y) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	leds[y][x][0] = 255 - leds[y][x][0];
	leds[y][x][1] = 255 - leds[y][x][1];
	leds[y][x][2] = 255 - leds[y][x][2];
	leds[y][x][3] = 1;
}
void getLedXY(uint8_t x, uint8_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
}

void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
{
	if(animationcount == MAX_ANIMATIONS)
		return;
	init_fp[animationcount] = init;
	tick_fp[animationcount] = tick;
	deinit_fp[animationcount] = deinit;
	duration[animationcount] = count;

	animationcount++;

	assert(t > 0);
	// 122Hz / tick
	//interval = 1000000 / 122 * t;
}

/*void registerApp(tick_fun tick,key_fun key)
{
	tick_fp = tick;
	key_fp = key;
}
*/


void fillRGB(uint8_t r,uint8_t g , uint8_t b)
{
	int x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			leds[y][x][0]=r;
			leds[y][x][1]=g;
			leds[y][x][2]=b;
			leds[y][x][3]=1;
		}
	}
}

int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	fillRGB(0,0,0);

	char nick[] = "hello World";

	srand(time(NULL));


	int current_animation = 0;
	
	screen = SDL_SetVideoMode(287,606,32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *image;
	image=IMG_Load("lun1k.png");

	SDL_BlitSurface(image,0,screen,0);
	SDL_Flip(screen);

	init_fp[current_animation]();
	
	int tick_count = 0;
	int running = 1;
	//unsigned long long int startTime = get_clock();
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							if(sdlpause == 0)
							{
								sdlpause = 1;
							}
							else{
								sdlpause = 0;
							}
							break;
						case SDLK_1:
							//key_fp(1);
							break;
						case SDLK_2:
							//key_fp(2);
							break;
						case SDLK_3:
							//key_fp(3);
							break;
						case SDLK_4:
							//key_fp(4);
							break;
						case SDLK_5:
							//key_fp(5);
							break;
						case SDLK_6:
							//key_fp(6);
							break;
						case SDLK_7:
							//key_fp(7);
							break;
						case SDLK_8:
							//key_fp(8);
							break;
						case SDLK_9:
							//key_fp(9);
							break;
						case SDLK_0:
							//key_fp(0);
							break;
							
						default: break;
					}
				default: break;
			}
		}

		tick_fp[current_animation](nick);


	


		
		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				if(leds[y][x][3] == 1)
				{

					SDL_Rect rect = { x+80, y+130, 1,1 };
					SDL_FillRect(
						screen, 
						&rect, 
						SDL_MapRGB(screen->format, leds[y][x][0],leds[y][x][1],leds[y][x][2])
					);
					leds[y][x][3] = 0;

				}

			}
		}

		//startTime+=interval;
		//int delay = startTime-get_clock();
//		if(delay > 0)
//			usleep(delay);
		
		SDL_Flip(screen);
		
		
		
		tick_count++;


		if(tick_count == duration[current_animation])
		{
			deinit_fp[current_animation]();

			current_animation++;
			if(current_animation == animationcount)
			{
				current_animation = 0;
			}
			tick_count=0;
	
			fillRGB(0,0,0);

			init_fp[current_animation]();


		}
	}

	SDL_Quit();
	return 0;
}

