#include "ds18b20.h"
#include "one_wire.h"

uint8_t ds18b20Init(void)
{
    return oneWireInit();
}

void ds18b20MatchRom()
{
    oneWireResetAndCheck();
    oneWireWriteByte(0X55);
}

uint8_t ds18b20ReadId(uint8_t *ds18b20_id)
{

    oneWireWriteByte(0x33);

    for (int i = 0; i < 8; i++)
    {
        ds18b20_id[i] = oneWireReadByte();
    }
    return ds18b20Crc8Calc(ds18b20_id, 7);
}

/// @brief
/// @param ds18b20_id  device id buffer
/// @param temp    get temp
/// @return  1 error 0 success
float ds18b20GetTemp(uint8_t *ds18b20_id)
{
    uint8_t tpmsb, tplsb, i;
    uint8_t regData[9] = {0};
    short s_tem;
    float f_tem;

    ds18b20MatchRom();

    for (i = 0; i < 8; i++)
        oneWireWriteByte(ds18b20_id[i]);

    oneWireWriteByte(0X44);

    ds18b20MatchRom();

    for (i = 0; i < 8; i++)
        oneWireWriteByte(ds18b20_id[i]);

    oneWireWriteByte(0XBE);
    for (int i = 0; i < 9; i++)
    {
        regData[i] = oneWireReadByte();
    }
    if (ds18b20Crc8Calc(regData, 8))
        return 1;

    tplsb = regData[0];
    tpmsb = regData[1];
    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    if (s_tem < 0)
        f_tem = (~s_tem + 1) * 0.0625;
    else
        f_tem = s_tem * 0.0625;
    return f_tem;
}

uint8_t reflect_byte(uint8_t data)
{
    uint8_t result = 0;
    for (int i = 0; i < 8; i++)
    {
        result = (result << 1) | (data & 0x01);
        data >>= 1;
    }
    return result;
}

uint8_t ds18b20Crc8Calc(uint8_t *data, int len)
{
    uint8_t crc = 0x00;        // INIT = 0x00
    const uint8_t poly = 0x8C; // 0x31 按位反转后的多项式
    uint8_t byte = 0X00;
    for (int i = 0; i < len; i++)
    {
        byte = reflect_byte(data[i]);
        crc ^= byte;
        for (int i = 0; i < 8; i++)
        {
            if (crc & 0x01)
            {
                crc = (crc >> 1) ^ poly;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    crc = reflect_byte(crc);
    crc ^= 0x00;
    return crc == data[len];
}
