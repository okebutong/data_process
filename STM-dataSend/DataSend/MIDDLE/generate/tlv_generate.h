#ifndef __TLVGENERATE_H
#define __TLVGENERATE_H

#include "stm32f1xx_hal.h"


uint8_t generateTlvData(uint8_t *sensorDataBuf, uint8_t *id, uint8_t *tempBuf, int type);


#endif
