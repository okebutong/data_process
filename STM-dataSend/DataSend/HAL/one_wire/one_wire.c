#include "one_wire.h"




uint8_t oneWireInit()
{
    return oneWireResetAndCheck();
}

void oneWireSetHigh()
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
}

void oneWireSetLow()
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
}

uint8_t oneWireReadPinBit()
{
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
}
// 此处由于使用hal库导致在改变GPIO为输入时延时过大，后采用寄存器直接写入后，顺利读取数据
void oneWireSetInputMode()
{
    GPIOE->CRL &= ~GPIO_CRL_MODE6;
    GPIOE->CRL &= ~GPIO_CRL_CNF6_0;
    GPIOE->CRL |= GPIO_CRL_CNF6_1;
    GPIOE->BSRR |= GPIO_BSRR_BR5;
}

void oneWireSetOutputMode()
{
    GPIOE->CRL |= GPIO_CRL_MODE6;
    GPIOE->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOE->CRL |= GPIO_CRL_CNF6_0;
    GPIOE->BSRR = GPIO_BSRR_BS6;
}

uint8_t oneWireResetAndCheck()
{
    uint8_t time = 0;
    oneWireSetOutputMode();
    oneWireSetLow();
    OneWire_Delay_us(500);

    oneWireSetInputMode();

    while ((oneWireReadPinBit() == GPIO_PIN_SET))
    {
        OneWire_Delay_us(1);
    }

    for (int i = 0; i < 150; i++)
    {
        if (!oneWireReadPinBit())
            time++;
        OneWire_Delay_us(1);
    }

    if (time >= 60)
        return 1;
    else
        return 0;
}

void oneWireWriteByte(uint8_t byte)
{
    uint8_t i, bit;
    oneWireSetOutputMode();

    for (i = 0; i < 8; i++)
    {
        bit = byte & 0x01;
        byte = byte >> 1;
        if (bit & 0x01)
        {
            oneWireSetLow();
            OneWire_Delay_us(10);
            oneWireSetHigh();
            OneWire_Delay_us(65);
        }
        else
        {
            oneWireSetLow();
            OneWire_Delay_us(65);
            oneWireSetHigh();
            OneWire_Delay_us(2);
        }

        OneWire_Delay_us(1);
    }
}

uint8_t oneWireReadBit()
{
    uint8_t dat;
    oneWireSetOutputMode();
    oneWireSetLow();
    OneWire_Delay_us(10);
    oneWireSetOutputMode();

    if (oneWireReadPinBit() == GPIO_PIN_SET)
        dat = 1;
    else
        dat = 0;

    OneWire_Delay_us(50);
    return dat;
}

uint8_t oneWireReadByte()
{
    uint8_t i, BitVal = 0, data = 0;

    for (i = 0; i < 8; i++)
    {
        BitVal = oneWireReadBit();
        data |= (BitVal << i);
    }

    return data;
}
