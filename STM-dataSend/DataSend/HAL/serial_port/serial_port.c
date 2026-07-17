#include "serial_port.h"
#include "stm32f1xx_hal.h"


void sendByteArray(unsigned char *data, int len)
{
	for(int i = 0; i < len; i++)
	{
		USART1->DR = data[i];
		while(!(USART1->SR & USART_SR_TC)){}
	}
}



