#include "main.h"
#include "arm_math.h" 

//#include "lib/usb_serial.h"

/*
 *	boot loader: http://www.st.com/stonline/stappl/st/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/APPLICATION_NOTE/CD00167594.pdf (page 31)
 *  data sheet : http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00277537.pdf
 *
 *
 */

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*10;

	while(TimingDelay != 0);
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


#define MAX_ANIMATIONS 30

static int animationcount;



struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t duration;
	uint16_t timing;
};

static struct animation animations[MAX_ANIMATIONS] CCM_ATTRIBUTES;



void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
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
 *      10   11
 *
 * E    C7   C5
 * DC   A7   A7
 * RW   C9   C9
 * RST  A6   A6
 * D0   C4   B15
 * D1   C5   B0
 * D2   B0   B1
 * D3   B1   B2
 * D4   B15  B14
 * D5   B14  B13
 * D6   B13  B12
 * D7   B12  C6
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


	lcdInit();
//	n35p112_init();

	
	int current_animation = 0;
	animations[current_animation].init_fp();
	int tick_count = 0;
	

	int loopcount = 0;

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
		


		animations[current_animation].tick_fp();


		lcdFlush();


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


		}
	}

}
