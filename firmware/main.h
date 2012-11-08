#ifndef MAIN_H_
#define MAIN_H_

#if STM32F == 2
#include "stm32f2xx.h"
#endif
#if STM32F == 4
#include "stm32f4xx.h"
#endif
#include "ssd1351.h"

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration);

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#if STM32F == 4
#define CCM_ATTRIBUTES	__attribute__ ((section(".ccm")));
#else
#define CCM_ATTRIBUTES	
#endif

#endif
