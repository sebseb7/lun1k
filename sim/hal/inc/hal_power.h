#ifndef _HAL_POWER_H
#define _HAL_POWER_H


uint16_t lun1k_get_battery_voltage(void); // 300..420
float lun1k_get_battery_charge(void); // 0..1
uint16_t lun1k_display_powerdown(void);
uint16_t lun1k_display_powerup(void);

#endif
