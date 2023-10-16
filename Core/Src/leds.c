#include "leds.h"
#include "main.h"

//Размер массива данных для индикаторов и светодиодов.
#define LED_DATA_SIZE		9u
//Индексы в массиве данных для индикаторов и светодиодов.
#define SPO2_FIRST_DIGIT	6u
#define SPO2_SECOND_DIGIT	7u
#define SPO2_THIRD_DIGIT	5u
#define HR_FIRST_DIGIT		1u
#define HR_SECOND_DIGIT		2u
#define HR_THIRD_DIGIT		0u
#define BARGRAPH_LEFT		3u
#define BARGRAPH_RIGHT		8u
#define MIX_LEDS			4u

//Массив данных для индикаторов и светодиодов.
volatile uint8_t ledData[LED_DATA_SIZE];

void ledsInit(void)
{
	//Индикаторы горят.
	ledData[SPO2_FIRST_DIGIT]	= 0x0;
	ledData[SPO2_SECOND_DIGIT]	= 0x0;
	ledData[SPO2_THIRD_DIGIT]	= 0x0;
	//Индикаторы горят.
	ledData[HR_FIRST_DIGIT]		= 0x0;
	ledData[HR_SECOND_DIGIT]	= 0x0;
	ledData[HR_THIRD_DIGIT]		= 0x0;
	//Светодиоды не горят.
	ledData[BARGRAPH_LEFT]		= 0xFF;
	ledData[BARGRAPH_RIGHT]		= 0xFF;
	ledData[MIX_LEDS]			= 0x71;
}

void ledsHandler(void)
{
	for(int8_t i = LED_DATA_SIZE; i >= 0; --i)
	{
		uint8_t byteToTransmit = ledData[i];
		for(uint8_t j = 0; j < 8; ++j)
		{
			uint8_t bitToTransmit = byteToTransmit & 0x1;
			if(bitToTransmit == 0)
			{
				HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin,
								  GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin,
								  GPIO_PIN_SET);
			}

			HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
			for(uint8_t i = 0; i < 200; ++i){}
			HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

			byteToTransmit = byteToTransmit >> 1;
		}
	}

	HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
}
