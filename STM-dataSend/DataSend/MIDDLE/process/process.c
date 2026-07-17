#include "process.h"
#include "ds18b20.h"
#include "tlv_generate.h"
#include "serial_port.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "semphr.h"

extern osSemaphoreId getTempStartHandle;
extern osSemaphoreId getTempEndHandle;
extern osMessageQId sensorDataQueueHandle;
extern osMessageQId tempQueueHandle;
uint8_t id[20] = {0};

void float2BigEndian(float value, uint8_t *buf);

void deviceInit()
{
	if (ds18b20Init())
		printf("init succ");

	ds18b20ReadId(id);
	//	int i = 0;
	//	while(id[i])
	//	{
	//		printf("%x ", id[i]);
	//		i++;
	//	}
}

void getTempTask(void *argument)
{
	/* USER CODE BEGIN getTempTask */
	osStatus_t xReturn = osErrorParameter;
	uint8_t tempDataBuf[4] = {0};
	/* Infinite loop */
	for (;;)
	{
		xReturn = osSemaphoreAcquire(getTempStartHandle, portMAX_DELAY);
		if (xReturn == osOK)
		{
			float temp = ds18b20GetTemp(id);
//			printf("%f ", temp);
			//			float2BigEndian(temp, tempDataBuf);
			//		for(int i = 0; i< 4; i++)
			//			printf("%x ",tempDataBuf[i]);
			xReturn = osMessageQueuePut(tempQueueHandle, &temp, 1, 0);
			if (xReturn == osOK)
			{
//				printf("message succ");
				continue;
			}
		}

		osDelay(1);
	}
	/* USER CODE END getTempTask */
}

void generateTlvDataTask(void *argument)
{
	/* USER CODE BEGIN generateTlvDataTask */
	osStatus_t xReturn = osErrorParameter;
	float temp = 0.0;
	uint8_t sensorDataBuf[10] = {0};
	uint8_t tempDataBuf[4] = {0};
	/* Infinite loop */
	for (;;)
	{
		xReturn = osMessageQueueGet(tempQueueHandle, &temp, (uint8_t *)1, portMAX_DELAY); //此处由于prio忽略，故在此处设为定值
		if (xReturn == osOK)
		{
			float2BigEndian(temp, tempDataBuf);
			
			generateTlvData(sensorDataBuf, id, tempDataBuf, 1);

			sendByteArray(sensorDataBuf, 10);
		}

		osDelay(1);
	}
	/* USER CODE END generateTlvDataTask */
}

// void startGetTemp()
//{
//	// 定时器开始启动，定期获取时间，在中断函数中改变flag的值，而后开始读取数据，读取完数据后，接着生成tlv数据
//	// 而后开始发送数据
//	//此处可以设置为多个任务，其中有温度获取任务，每个设备为一个任务，
//	//另外，温度转换为一个任务，
//	float temp = 0.0;
//	uint8_t sensorDataBuf[10] = {0};
//
//	ds18b20GetTemp(id, &temp);
////	printf("temp %f\n", temp);
//	generateTlvData(id, 1, temp, sensorDataBuf);
////	for (int i = 0; i < 10; i++)
////	{
////		printf("%x ", sensorDataBuf[i]);
////	}
//	sendByteArray(sensorDataBuf, 10);
//
//}

void getTempTimerCallback(void *argument)
{
	/* USER CODE BEGIN getTempTimerCallback */
	osSemaphoreRelease(getTempStartHandle);
	/* USER CODE END getTempTimerCallback */
}
void float2BigEndian(float value, uint8_t *buf)
{
	uint8_t *p = (uint8_t *)&value;

	buf[0] = p[3];
	buf[1] = p[2];
	buf[2] = p[1];
	buf[3] = p[0];

	// 后续添加读取错误的判断
}