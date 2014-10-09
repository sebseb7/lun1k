#include <stdlib.h>

#include "main.h"
#include "libs/mcugui/circle.h"
#include "arm_math.h" 
#include "n35p112.h"
#include "rng.h"
#include "libs/mcugui/text.h"
#include "adc.h"


//#include "lib/usb_serial.h"

/*
 *	boot loader: http://www.st.com/stonline/stappl/st/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/APPLICATION_NOTE/CD00167594.pdf (page 31)
 *	datasheet: http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
 *	user manual: http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf
 */

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*10;

	while(TimingDelay != 0);
}
static void Delay100us(__IO int32_t nTime)
{

	if(nTime > 0)
	{

		TimingDelay = nTime;

		while(TimingDelay != 0);
	}
}



static uint16_t key_state;
static uint16_t key_press;
uint32_t buttonsInitialized = 0;


void TimingDelay_Decrement(void)
{
	static uint16_t ct0, ct1;
	static uint8_t button_sample = 0;
	uint16_t i;


	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	tick++;
	if(buttonsInitialized)
	{
		button_sample++;
		if(button_sample < 100)
			return;

		button_sample = 0;

		uint16_t key_curr = ((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)^1)<<1)|
							(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)<<2)|
							(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)<<3)|
							GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

		i = key_state ^ ~key_curr;
		ct0 = ~( ct0 & i );
		ct1 = ct0 ^ (ct1 & i);
		i &= ct0 & ct1;
		key_state ^= i;
		key_press |= key_state & i;
	}
}
uint32_t getSysTick(void)
{
	return tick;
}
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

static uint32_t i2c_e[7] = {0,0,0,0,0,0,0};

void i2c_error(uint8_t error)
{
	i2c_e[error-1]++;
}


#define MAX_ANIMATIONS 30

static int animationcount;



struct animation {
	char* name;
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t duration;
	int16_t timing;
};

static struct animation animations[MAX_ANIMATIONS] ;

static uint8_t joy_x=0;
static 	uint8_t joy_y=0;

void get_stick(uint8_t *x,uint8_t *y)
{
	*x = joy_x;
	*y = joy_y;
}

void registerAnimation(char* name,init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
{

	// this is for initialization, probably registerAnimation gets called bevore global variables are initialized
	static int firstrun = 1;

	if(firstrun == 1)
	{
		firstrun = 0;
		animationcount = 0;
	}


	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].duration = count;
	animations[animationcount].timing = t;
	animations[animationcount].name = name;

	animationcount++;
}


int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;


	RCC_GetClocksFreq(&RCC_Clocks);
	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	
	//prepare init structure
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
/*
 *      10   11   12
 *
 * E    C7   C5   B12
 * DC   A7   A7   A7
 * RW   C9   C9   C9
 * RST  A6   A6   A6
 * D0   C4   B15  B15
 * D1   C5   B0   B0
 * D2   B0   B1   B1
 * D3   B1   B2   B2
 * D4   B15  B14  B14
 * D5   B14  B13  B13
 * D6   B13  B12  C5
 * D7   B12  C6   C6
 *
 * v11 : E,D0..D7 swapped
 * v12 : E and D6 swapped
 *
 */

	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#if LUN1K_VERSION >= 11
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#if LUN1K_VERSION == 10
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#if LUN1K_VERSION >= 11
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#if LUN1K_VERSION == 10
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//leds
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;       
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	//regen
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  


	//ESC Button
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//stick button
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	//A
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//B
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	buttonsInitialized=1;
		


	//set RW to 0
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);	

	// 12V power
	GPIOA->ODR           |=       1<<1;


	RNG_Enable();
	srand(RNG_Get());
	RNG_Disable();

	adc_a3_init();

	lcdInit();
	
	n35p112_init();

	
	int current_animation = 0;
	animations[current_animation].init_fp();
	int tick_count = 0;
	

	int loopcount = 0;

	uint8_t count  = 0;
		
	int16_t bat_voltage = adc_a3_get();

	float voltagesample = bat_voltage;

	while(1)
	{
		loopcount++;
		if((loopcount == 55)||(loopcount == 57))
		{
			GPIOC->ODR           |=       1<<1;
			GPIOD->ODR           |=       1<<2;
			GPIOB->ODR           |=       1<<3;
			GPIOC->ODR           |=       1<<3;
		}
		if((loopcount == 56)||(loopcount == 58))
		{
			GPIOC->ODR           &=       ~(1<<1);
			GPIOD->ODR           &=       ~(1<<2);
			GPIOB->ODR           &=       ~(1<<3);
			GPIOC->ODR           &=       ~(1<<3);
			if(loopcount==58)
				loopcount = 0;
		}
		
		uint32_t start_tick = tick;

		get_n35p112(&joy_x,&joy_y);

		animations[current_animation].tick_fp();
/*		int16_t bat_voltage = adc_a3_get();

		fill_8x6(20,20, 5,0,0,0);
		fill_8x6(20,30, 5,0,0,0);
		draw_number_8x6(20,20, bat_voltage, 5, ' ' ,255,255,255);

		voltagesample = voltagesample * 0.98f;
		voltagesample += bat_voltage * 0.02f;


		float tmp2 = voltagesample / (4096.0f / 3.3f);

		float tmp3 = tmp2 * 2.14f;

		draw_number_8x6(20,30, tmp3*1000, 5, ' ' ,255,255,255);

		setLedXY(bat_voltage-2364, count,255,0,0);*/

		lcdFlush();

		uint32_t duration = tick - start_tick;

		//draw_number_8x6(20,20, animations[current_animation].timing - duration, 6, ' ' ,255,255,255);


		if(animations[current_animation].timing > 0)
			Delay100us(animations[current_animation].timing - duration);
	

//		draw_number_8x6(20,30, joy_y, 3, ' ' ,255,255,255);

//		draw_filledCircle(joy_y>>1,joy_x>>1,8,0,0,0);
//		draw_filledCircle(joy_y>>1,joy_x>>1,5,255,255,255);


		count++;
		if(count > 128)
			count=0;

		tick_count++;


		if(get_key_press(KEY_ESC))
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

			//usb_printf("diff: %i , %i (%i) (%i %i %i %i %i %i %i)\n",diff,int_status,i2c_errors,i2c_e[0],i2c_e[1],i2c_e[2],i2c_e[3],i2c_e[4],i2c_e[5],i2c_e[6]);

		}
	}

}
