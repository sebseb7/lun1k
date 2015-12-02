#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <dlfcn.h>
#include <stdio.h>

#include "main.h"


#include <string.h>

#include<sys/time.h>
#include <dlfcn.h>

#ifndef NOMALLOCWRAP

#define MAX_HEAP_SIZE 114688 //112k

static size_t gnCurrentMemory = 0;
static size_t gnPeakMemory    = 0;

void *xmalloc (size_t nSize)
{
	void *(*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
	void *pMem = libc_malloc(nSize + sizeof(size_t));

	if(gnCurrentMemory+nSize > MAX_HEAP_SIZE)
	{
		printf("out of memory (%lu)\n",(long int)nSize);
		return NULL;
	}


	if (pMem)
	{
		size_t *pSize = (size_t *)pMem;
		memcpy(pSize, &nSize, sizeof(nSize));
		gnCurrentMemory += nSize;
		if (gnCurrentMemory > gnPeakMemory)
		{
			gnPeakMemory = gnCurrentMemory;
		}

		printf("malloc - Size (%lu), Current (%lu), Peak (%lu)\n", (long unsigned int)nSize, (long unsigned int)gnCurrentMemory, (long unsigned int)gnPeakMemory);
		return(pSize + 1);
	}
	return NULL;
}

void  xfree (void *pMem)
{
	if(pMem)
	{
		size_t *pSize = (size_t *)pMem;
		--pSize;

		assert(gnCurrentMemory >= *pSize);

		gnCurrentMemory -= *pSize;
		
		printf("free - Size (%lu), Current (%lu), Peak (%lu)\n",  (long unsigned int)*pSize, (long unsigned int)gnCurrentMemory, (long unsigned int)gnPeakMemory);

		void (*libc_free)(void*) = dlsym(RTLD_NEXT, "free");
		libc_free(pSize);
	}
}

#endif

#define FRAMETIME 33

int sdlpause = 0;

#define MAX_ANIMATIONS 200
#define MAX_APPS 200

int animationcount = 0;
int appcount = 0;

struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int duration;
	int min_delay;
} animations[MAX_ANIMATIONS];


struct app {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int min_delay;
} apps[MAX_APPS];


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

void registerAnimation(char * name,init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
{
	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].duration = count;
	animations[animationcount].min_delay = t;

	animationcount++;

}

static uint16_t key_press;
static uint16_t key_dir;

uint16_t get_key_press( uint16_t key_mask )
{
	key_mask &= key_press;                          // read key(s)
	key_press ^= key_mask;                          // clear key(s)
	return key_mask;
}
uint16_t get_key_state( uint16_t key_mask )
{
	return key_mask & key_press;
}

static uint8_t joy_x=128;
static 	uint8_t joy_y=128;

void get_stick(uint8_t *x,uint8_t *y)
{
	*x = joy_x;
	*y = joy_y;
}



uint32_t getSysTick(void)
{
	return SDL_GetTicks()*10;
}


void lcdFillRGB2(uint8_t r,uint8_t g , uint8_t b)
{
	lcdFillRGB(r,g,b);
}
void lcdFillRGB(uint8_t r,uint8_t g , uint8_t b)
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
static int hatposition=SDL_HAT_CENTERED;


int joy_is_up(void)
{
	if((hatposition&SDL_HAT_UP)||(key_dir&KEY_UP))
	{
		return 1;
	}
	return 0;
}
int joy_is_down(void)
{
	if((hatposition&SDL_HAT_DOWN)||(key_dir&KEY_DOWN))
	{
		return 1;
	}
	return 0;
}
int joy_is_left(void)
{
	if((hatposition&SDL_HAT_LEFT)||(key_dir&KEY_LEFT))
	{
		return 1;
	}
	return 0;
}
int joy_is_right(void)
{
	if((hatposition&SDL_HAT_RIGHT)||(key_dir&KEY_RIGHT))
	{
		return 1;
	}
	return 0;
}


void flash_db_write(uint16_t addr,uint16_t data)
{
}


void flash_db_read(uint16_t addr,uint16_t *data)
{

	uint16_t read_data=0;
	*data = read_data;

}



int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	lcdFillRGB(0,0,0);

	srand(time(NULL));

	SDL_Init(SDL_INIT_JOYSTICK);

	printf("joysticks: %i",SDL_NumJoysticks());
	SDL_JoystickOpen(0);
	SDL_JoystickEventState(SDL_ENABLE);

	int current_animation = 0;

	screen = SDL_SetVideoMode(287,606,32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *image;
	image=IMG_Load("lun1k.png");

	SDL_BlitSurface(image,0,screen,0);
	SDL_Flip(screen);

	animations[current_animation].init_fp();

	int tick_count = 0;
	int running = 1;
	//unsigned long long int startTime = get_clock();
	Uint32 lastFrame = SDL_GetTicks(); 

	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_JOYHATMOTION:
					hatposition = ev.jhat.value;
					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym) {
						case SDLK_LEFT:
							key_dir &= ~KEY_LEFT;
							break;
						case SDLK_RIGHT:
							key_dir &= ~KEY_RIGHT;
							break;
						case SDLK_UP:
							key_dir &= ~KEY_UP;
							break;
						case SDLK_DOWN:
							key_dir &= ~KEY_DOWN;
							break;

						default: break;
					}
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
							key_press |= KEY_ESC;
							break;
						case SDLK_2:
							key_press |= KEY_B;
							break;
						case SDLK_3:
							key_press |= KEY_A;
							break;
						case SDLK_4:
							key_press |= KEY_STICK;
							break;
						case SDLK_LEFT:
							key_dir |= KEY_LEFT;
							break;
						case SDLK_RIGHT:
							key_dir |= KEY_RIGHT;
							break;
						case SDLK_UP:
							key_dir |= KEY_UP;
							break;
						case SDLK_DOWN:
							key_dir |= KEY_DOWN;
							break;

						default: break;
					}
					break;
				default: 
					printf("unknown type:%i\n",ev.type);
					break;
			}
		}

		animations[current_animation].tick_fp();






		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				if(leds[y][x][3] == 1)
				{

					SDL_Rect rect = { x+80, y+130, 1,1 };
					SDL_FillRect(
							screen, 
							&rect, 
							SDL_MapRGB(screen->format, leds[y][x][0] & 0xFC ,leds[y][x][1] & 0xFC ,leds[y][x][2] & 0xFC)
							);
					leds[y][x][3] = 0;

				}

			}
		}


		SDL_Flip(screen);


		Uint32 now = SDL_GetTicks(); 

		if( (now - lastFrame) < FRAMETIME )
		{
			SDL_Delay(FRAMETIME - (now - lastFrame));
		}
		lastFrame = SDL_GetTicks();


		tick_count++;


		if(tick_count == animations[current_animation].duration)
		{
			animations[current_animation].deinit_fp();

			current_animation++;
			if(current_animation == animationcount)
			{
				current_animation = 0;
			}
			tick_count=0;

			lcdFillRGB(0,0,0);

			animations[current_animation].init_fp();


		}
	}

	SDL_Quit();
	return 0;
}

