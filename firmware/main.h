#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "drivers/ssd1351.h"

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
uint32_t getSysTick(void);

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration);

#define LUN1K

#define KEY_A (1<<0)
#define KEY_ESC (1<<1)
#define KEY_STICK (1<<2)
#define KEY_B (1<<3)

uint16_t get_key_press( uint16_t key_mask );
uint16_t get_key_state( uint16_t key_mask );

void i2c_error(uint8_t);

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#define CCM_ATTRIBUTES	__attribute__ ((section(".ccm")));

#endif
