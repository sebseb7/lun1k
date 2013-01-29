#ifndef _RNG_H
#define _RNG_H

#include <stdint.h>

uint32_t RNG_Get(void);
void RNG_Enable(void);
void RNG_Disable(void);

#endif
