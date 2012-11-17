#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "ssd1351.h"

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration);

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#define CCM_ATTRIBUTES	__attribute__ ((section(".ccm")));

#endif
