#include "display.h"
#include "main.h"
#include <stdbool.h>


//Размер массива данных.
#define DISPLAY_DATA_SIZE		9u
//Индексы в массиве данных.
#define DISPLAY1_FIRST_DIGIT	6u
#define DISPLAY1_SECOND_DIGIT	7u
#define DISPLAY1_THIRD_DIGIT	5u
#define DISPLAY2_FIRST_DIGIT	1u
#define DISPLAY2_SECOND_DIGIT	2u
#define DISPLAY2_THIRD_DIGIT	0u
#define LEDS_LEFT				3u
#define LEDS_RIGHT				8u
#define MIX_LEDS				4u
//Значения данных для индикаторов.
#define DISPLAY_EMPTY			0xFF
#define DISPLAY_DIGIT_0			0x88
#define DISPLAY_DIGIT_1			0xEE
#define DISPLAY_DIGIT_2			0x94
#define DISPLAY_DIGIT_3			0xC4
#define DISPLAY_DIGIT_4			0xE2
#define DISPLAY_DIGIT_5			0xC1
#define DISPLAY_DIGIT_6			0x81
#define DISPLAY_DIGIT_7			0xEC
#define DISPLAY_DIGIT_8			0x80
#define DISPLAY_DIGIT_9			0xC0
#define DISPLAY_FULL			0x0
//Значения данных для светодиодов.
#define LEDS_OFF				0xFF
#define MIX_LEDS_OFF			0x71


//Массив данных для индикаторов и светодиодов.
volatile uint8_t displayData[DISPLAY_DATA_SIZE]; //TODO Общий ресурс.


static uint16_t getVoltage();
static uint16_t getLux();
static void convertVoltageToDisplayData(uint16_t voltage);
static void convertLuxToDisplayData(uint16_t lux);
static void convertParameterToDisplayData(uint16_t parameter,
								   	   	  uint8_t firstDigitIndex,
										  uint8_t secondDigitIndex,
										  uint8_t thirdDigitIndex);
static uint8_t convertDigitToDisplayData(uint8_t digit);
static void transmitDisplayData();


void displayInit()
{
	//TODO Общий ресурс.
	//Индикаторы не горят.
	displayData[DISPLAY1_FIRST_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY1_SECOND_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY1_THIRD_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_FIRST_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_SECOND_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_THIRD_DIGIT]	= DISPLAY_EMPTY;
	//Светодиоды не горят.
	displayData[LEDS_LEFT]	= LEDS_OFF;
	displayData[LEDS_RIGHT]	= LEDS_OFF;
	displayData[MIX_LEDS]	= MIX_LEDS_OFF;
}

//TODO Общий ресурс. Сделать задачей RTOS либо вызывать периодично от таймера. Сделать передачу по прерываниям. Убрать программную задержку.
void displayHandler()
{
	while(true)
	{
		uint16_t voltage = getVoltage();
		uint16_t lux = getLux();
		convertVoltageToDisplayData(voltage);
		convertLuxToDisplayData(lux);
		transmitDisplayData();
		for(uint32_t i = 0; i < 100000; ++i){}
	}
}

static uint16_t getVoltage()
{
	static uint16_t voltage = 0;

	uint16_t temp = voltage;

	++voltage;
	if(voltage > 999)
	{
		voltage = 0;
	}

	return temp;
}

static uint16_t getLux()
{
	static uint16_t lux = 999;

	uint16_t temp = lux;

	--lux;
	if(lux > 999)
	{
		lux = 999;
	}

	return temp;
}

static void convertVoltageToDisplayData(uint16_t voltage)
{
	convertParameterToDisplayData(voltage,
						   	   	  DISPLAY1_FIRST_DIGIT,
								  DISPLAY1_SECOND_DIGIT,
								  DISPLAY1_THIRD_DIGIT);
}

static void convertLuxToDisplayData(uint16_t lux)
{
	convertParameterToDisplayData(lux,
						   	   	  DISPLAY2_FIRST_DIGIT,
								  DISPLAY2_SECOND_DIGIT,
								  DISPLAY2_THIRD_DIGIT);
}

static void convertParameterToDisplayData(uint16_t parameter,
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

	uint8_t firstDigitDisplayData = DISPLAY_EMPTY;
	uint8_t secondDigitDisplayData = DISPLAY_EMPTY;
	uint8_t thirdDigitDisplayData = DISPLAY_EMPTY;
	firstDigitDisplayData = convertDigitToDisplayData(firstDigit);
	secondDigitDisplayData = convertDigitToDisplayData(secondDigit);
	thirdDigitDisplayData = convertDigitToDisplayData(thirdDigit);

	//TODO Общий ресурс.
	displayData[firstDigitIndex] = firstDigitDisplayData;
	displayData[secondDigitIndex] = secondDigitDisplayData;
	displayData[thirdDigitIndex] = thirdDigitDisplayData;
}

static uint8_t convertDigitToDisplayData(uint8_t digit)
{
	uint8_t data = DISPLAY_EMPTY;

	switch(digit)
	{
		case 0:
			data = DISPLAY_DIGIT_0;
			break;
		case 1:
			data = DISPLAY_DIGIT_1;
			break;
		case 2:
			data = DISPLAY_DIGIT_2;
			break;
		case 3:
			data = DISPLAY_DIGIT_3;
			break;
		case 4:
			data = DISPLAY_DIGIT_4;
			break;
		case 5:
			data = DISPLAY_DIGIT_5;
			break;
		case 6:
			data = DISPLAY_DIGIT_6;
			break;
		case 7:
			data = DISPLAY_DIGIT_7;
			break;
		case 8:
			data = DISPLAY_DIGIT_8;
			break;
		case 9:
		default:
			data = DISPLAY_DIGIT_9;
			break;
	}

	return data;
}

//TODO Сделать через прерывания. Общий ресурс.
static void transmitDisplayData()
{
	for(int8_t i = DISPLAY_DATA_SIZE; i >= 0; --i)
	{
		uint8_t byteToTransmit = displayData[i];
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
