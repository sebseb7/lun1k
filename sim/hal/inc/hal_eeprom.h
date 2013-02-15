#ifndef _HAL_EEPROM_H
#define _HAL_EEPROM_H


//keep this as small as possible
#define EE_ADDRESS_SPACE_SIZE 20

uint16_t EE_ReadVariable(uint16_t Address, uint16_t* Data);
uint16_t EE_WriteVariable(uint16_t Address, uint16_t Data);


#endif
