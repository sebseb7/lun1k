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

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	tick++;
}

#define MAX_ANIMATIONS 30

static int animationcount;

static init_fun init_fp[MAX_ANIMATIONS] __attribute__ ((section(".ccm")));
static tick_fun tick_fp[MAX_ANIMATIONS] __attribute__ ((section(".ccm")));
static deinit_fun deinit_fp[MAX_ANIMATIONS] __attribute__ ((section(".ccm")));
static uint16_t duration[MAX_ANIMATIONS] __attribute__ ((section(".ccm")));
static uint16_t timing[MAX_ANIMATIONS] __attribute__ ((section(".ccm")));


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
	init_fp[animationcount] = init;
	tick_fp[animationcount] = tick;
	deinit_fp[animationcount] = deinit;
	duration[animationcount] = count;
	timing[animationcount] = t;

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
	
	
	GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO );

	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_5);



	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;


#ifdef lunik
#warning lunic_main
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC,GPIO_Pin_9);	

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;       
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// 12V power
	GPIOA->ODR           |=       1<<1;

#else
#warning non-lunic_main

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
#endif



/*
DC=A7
RST=A6
E=C7
RW=C9
D0=c4
D1=C5
D2=B0
D3=B1
D4=B15
D5=B14
D6=B13
D7=B12
*/


	lcdInit();

//	draw_number_8x6(20,20,animationcount,3,' ',255,255,255);
//	lcdFlush();
	
/*	RCC->CFGR &= ~RCC_CFGR_PPRE2;

	RCC->CFGR &= ~RCC_CFGR_PPRE1;

	RCC->CFGR |= RCC_CFGR_PPRE2_DIV16;

	RCC->CFGR |= RCC_CFGR_PPRE1_DIV16;
*/
/*
	while(1)
	{
			GPIOC->BSRRL = 1<<1;
			GPIOC->BSRRH = 1<<1;
//			GPIOC->ODR           |=       1<<1;
//			GPIOD->ODR           |=       1<<2;
//			GPIOB->ODR           |=       1<<3;
//			GPIOC->ODR           &=       ~(1<<1);
//			GPIOD->ODR           &=       ~(1<<2);
//			GPIOB->ODR           &=       ~(1<<3);
	}
*/
//	int count = 10;
//	int count2 = 0;
//	initTables();
	
	int current_animation = 0;
	init_fp[current_animation]();
	int tick_count = 0;
	
	while(1)
	{
		tick_fp[current_animation]();
		GPIOB->ODR           &=       ~(1<<12);
		lcdFlush();
		GPIOB->ODR           |=       1<<12;



//		count++;
//		if(count==100) count=10;
//		setLedXY(10,count,255,255,255);
//		setLedXY(20,count,255,0,0);
//		setLedXY(30,count,0,255,0);
//		setLedXY(40,count,0,0,255);


#ifndef lunik

//		GPIOB->ODR           &=       ~(1<<13);

//		lcdFillRGB(255,0,0);

//		tutick();
//		Delay(200);
//		GPIOB->ODR           &=       ~(1<<12);
//		tutick();
//		GPIOB->ODR           |=       1<<13;
//		lcdFillRGB(0,0,255);
//		Delay(200);
//		pltick();
//		pltick();
//		Delay(1080);
#else
		if(count==10)
		{
			count2++;
			if(count2 == 40)
			{
				count2=0;
			}
			//GPIOC->ODR           |=       1<<1;
			GPIOD->ODR           |=       1<<2;
			GPIOB->ODR           |=       1<<3;
		}
		if(count == 11)
		{
			//GPIOC->ODR           &=       ~(1<<1);
			GPIOD->ODR           &=       ~(1<<2);
			GPIOB->ODR           &=       ~(1<<3);
		}
		pltick();

		if(count2==10)
		{
			count2++;
			for(int i = 0; i < 100;i++)
			{
				lcdFillRGB(255,0,0);
				Delay(80);
				lcdFillRGB(0,0,255);
				Delay(80);
			
			};
		}
		if(count2==20)
		{
			count2++;
			for(int i = 0; i < 50;i++)
			{
				lcdFillRGB(255,255,255);
				Delay(2);
				lcdFillRGB(0,0,0);
				Delay(200);
			
			};
		}
		if(count2==30)
		{
			count2++;
			for(int i = 0; i < 20;i++)
			{
				lcdFillRGB(255,0,0);
				Delay(60);
				lcdFillRGB(0,0,0);
				Delay(600);
				lcdFillRGB(0,255,0);
				Delay(60);
				lcdFillRGB(0,0,0);
				Delay(600);
				lcdFillRGB(0,0,255);
				Delay(60);
				lcdFillRGB(0,0,0);
				Delay(600);
			
			};
		}


#endif



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
	
			lcdFillRGB(0,0,0);

			init_fp[current_animation]();


		}
	}

}
