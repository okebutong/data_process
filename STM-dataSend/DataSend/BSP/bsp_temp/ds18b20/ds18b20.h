#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f1xx_hal.h"

uint8_t ds18b20Init(void);
uint8_t ds18b20ReadId(uint8_t *ds18b20_id);
void ds18b20MatchRom(void);
float ds18b20GetTemp(uint8_t *ds18b20_id);
uint8_t ds18b20Crc8Calc(uint8_t *data, int len);

#endif
