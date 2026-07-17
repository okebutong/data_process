#ifndef __ONEWIRE_H_
#define __ONEWIRE_H_

#include "stm32f1xx_hal.h"
#include "core_delay.h"


#define OneWire_Delay_us(us) CPU_TS_Tmr_Delay_US(us)
#define OneWire_Delay_ms(ms) CPU_TS_Tmr_Delay_MS(ms)
void oneWireSetInputMode(void);
void oneWireSetOutputMode(void);
void oneWireSetHigh(void);
void oneWireSetLow(void);
uint8_t oneWireReadBit(void);
uint8_t oneWireReadPinBit(void);
uint8_t oneWireInit(void);
uint8_t oneWireResetAndCheck(void);
uint8_t oneWireReadByte(void);

void oneWireWriteByte(uint8_t bit);

#endif /*__a_H_*/
