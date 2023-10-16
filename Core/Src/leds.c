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
//Значения данных для индикаторов.
#define INDICATOR_EMPTY		0xFF
#define INDICATOR_DIGIT_0	0x88
#define INDICATOR_DIGIT_1	0xEE
#define INDICATOR_DIGIT_2	0x94
#define INDICATOR_DIGIT_3	0xC4
#define INDICATOR_DIGIT_4	0xE2
#define INDICATOR_DIGIT_5	0xC1
#define INDICATOR_DIGIT_6	0x81
#define INDICATOR_DIGIT_7	0xEC
#define INDICATOR_DIGIT_8	0x80
#define INDICATOR_DIGIT_9	0xC0
#define INDICATOR_FULL		0x0
//Значения данных для светодиодов.
#define LEDS_EMPTY			0xFF
#define MIX_LEDS_EMPTY		0x71

//Массив данных для индикаторов и светодиодов.
volatile uint8_t ledData[LED_DATA_SIZE];

static void convertVoltageToData(uint16_t voltage);
static void convertLuxToData(uint16_t lux);
static void convertParameterToData(uint16_t parameter,
								   uint8_t firstDigitIndex,
								   uint8_t secondDigitIndex,
								   uint8_t thirdDigitIndex);
static uint8_t convertDigitToData(uint8_t digit);

void ledsInit(void)
{
	//TODO Общий ресурс.
	//Индикаторы не горят.
	ledData[SPO2_FIRST_DIGIT]	= INDICATOR_EMPTY;
	ledData[SPO2_SECOND_DIGIT]	= INDICATOR_EMPTY;
	ledData[SPO2_THIRD_DIGIT]	= INDICATOR_EMPTY;
	ledData[HR_FIRST_DIGIT]		= INDICATOR_EMPTY;
	ledData[HR_SECOND_DIGIT]	= INDICATOR_EMPTY;
	ledData[HR_THIRD_DIGIT]		= INDICATOR_EMPTY;
	//Светодиоды не горят.
	ledData[BARGRAPH_LEFT]		= LEDS_EMPTY;
	ledData[BARGRAPH_RIGHT]		= LEDS_EMPTY;
	ledData[MIX_LEDS]			= MIX_LEDS_EMPTY;
}

void ledsHandlerTest(void)
{
	uint16_t upCounter = 0;
	uint16_t downCounter = 999;

	while(1)
	{
		convertVoltageToData(upCounter);
		convertLuxToData(downCounter);

		ledsHandler();

		++upCounter;
		if(upCounter > 999)
		{
			upCounter = 0;
		}
		--downCounter;
		if(downCounter > 999)
		{
			downCounter = 999;
		}

		for(uint32_t i = 0; i < 10000; ++i){}
	}
}

//TODO Общий ресурс. Засунуть в прерывание.
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

static void convertVoltageToData(uint16_t voltage)
{
	convertParameterToData(voltage,
						   SPO2_FIRST_DIGIT,
						   SPO2_SECOND_DIGIT,
						   SPO2_THIRD_DIGIT);
}

static void convertLuxToData(uint16_t lux)
{
	convertParameterToData(lux,
						   HR_FIRST_DIGIT,
						   HR_SECOND_DIGIT,
						   HR_THIRD_DIGIT);
}

static void convertParameterToData(uint16_t parameter,
								   uint8_t firstDigitIndex,
								   uint8_t secondDigitIndex,
								   uint8_t thirdDigitIndex)
{
	if(parameter > 999)
	{
		parameter = 999;
	}

	uint8_t firstDigit = 0;
	uint8_t secondDigit = 0;
	uint8_t thirdDigit = 0;
	thirdDigit = parameter%10;
	parameter = parameter/10;
	secondDigit = parameter%10;
	parameter = parameter/10;
	firstDigit = parameter%10;

	uint8_t firstDigitData = INDICATOR_EMPTY;
	uint8_t secondDigitData = INDICATOR_EMPTY;
	uint8_t thirdDigitData = INDICATOR_EMPTY;
	firstDigitData = convertDigitToData(firstDigit);
	secondDigitData = convertDigitToData(secondDigit);
	thirdDigitData = convertDigitToData(thirdDigit);

	//TODO Общий ресурс.
	ledData[firstDigitIndex] = firstDigitData;
	ledData[secondDigitIndex] = secondDigitData;
	ledData[thirdDigitIndex] = thirdDigitData;
}

static uint8_t convertDigitToData(uint8_t digit)
{
	uint8_t data = INDICATOR_EMPTY;

	switch(digit)
	{
		case 0:
			data = INDICATOR_DIGIT_0;
			break;
		case 1:
			data = INDICATOR_DIGIT_1;
			break;
		case 2:
			data = INDICATOR_DIGIT_2;
			break;
		case 3:
			data = INDICATOR_DIGIT_3;
			break;
		case 4:
			data = INDICATOR_DIGIT_4;
			break;
		case 5:
			data = INDICATOR_DIGIT_5;
			break;
		case 6:
			data = INDICATOR_DIGIT_6;
			break;
		case 7:
			data = INDICATOR_DIGIT_7;
			break;
		case 8:
			data = INDICATOR_DIGIT_8;
			break;
		case 9:
		default:
			data = INDICATOR_DIGIT_9;
			break;
	}

	return data;
}
