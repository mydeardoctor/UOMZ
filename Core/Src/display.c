#include <stdbool.h>
#include "main.h"
#include "tim.h"
#include "voltage.h"
#include "lux.h"
#include "display.h"
#include "cmsis_os.h"


#define DISPLAY_DATA_SIZE 9u


typedef enum
{
	CLK_RISING_EDGE,
	CLK_FALLING_EDGE,
	STROBE_RISING_EDGE,
	STROBE_FALLING_EDGE
} TransmissionState;


extern osMutexId_t mutexVoltageHandle;
extern osMutexId_t mutexLuxHandle;
extern osSemaphoreId_t semaphoreDisplayHandle;

//Массив данных для индикаторов и светодиодов.
volatile uint8_t displayData[DISPLAY_DATA_SIZE];
//Индексы в массиве данных.
const uint8_t DISPLAY1_FIRST_DIGIT	= 6u;
const uint8_t DISPLAY1_SECOND_DIGIT	= 7u;
const uint8_t DISPLAY1_THIRD_DIGIT	= 5u;
const uint8_t DISPLAY2_FIRST_DIGIT	= 1u;
const uint8_t DISPLAY2_SECOND_DIGIT	= 2u;
const uint8_t DISPLAY2_THIRD_DIGIT	= 0u;
const uint8_t LEDS_LEFT				= 3u;
const uint8_t LEDS_RIGHT			= 8u;
const uint8_t MIX_LEDS				= 4u;
//Значения данных для индикаторов.
const uint8_t DISPLAY_EMPTY			= 0xFF;
const uint8_t DISPLAY_DIGIT_0		= 0x88;
const uint8_t DISPLAY_DIGIT_1		= 0xEE;
const uint8_t DISPLAY_DIGIT_2		= 0x94;
const uint8_t DISPLAY_DIGIT_3		= 0xC4;
const uint8_t DISPLAY_DIGIT_4		= 0xE2;
const uint8_t DISPLAY_DIGIT_5		= 0xC1;
const uint8_t DISPLAY_DIGIT_6		= 0x81;
const uint8_t DISPLAY_DIGIT_7		= 0xEC;
const uint8_t DISPLAY_DIGIT_8		= 0x80;
const uint8_t DISPLAY_DIGIT_9		= 0xC0;
const uint8_t DISPLAY_FULL			= 0x0;
//Значения данных для светодиодов.
const uint8_t LEDS_OFF				= 0xFF;
const uint8_t MIX_LEDS_OFF			= 0x71;


static uint16_t getVoltage();
static uint16_t getLux();
static void convertVoltageToDisplayData(uint16_t voltage);
static void convertLuxToDisplayData(uint16_t lux);
static void convertParameterToDisplayData(uint16_t parameter,
								   	   	  uint8_t firstDigitIndex,
										  uint8_t secondDigitIndex,
										  uint8_t thirdDigitIndex);
static uint8_t convertDigitToDisplayData(uint8_t digit);
static void startTransmissionOfDisplayData();
static void waitForEndOfTransmission();


void displayInit() //TODO инициализация внутри задачи
{
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

void taskDisplayFunction(void *argument)
{
	osSemaphoreAcquire(semaphoreDisplayHandle, osWaitForever);

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		uint16_t voltage_ = getVoltage();
		uint16_t lux_ = getLux();
		convertVoltageToDisplayData(voltage_);
		convertLuxToDisplayData(lux_);
		startTransmissionOfDisplayData();
		waitForEndOfTransmission();

		osDelayUntil(tick + pdMS_TO_TICKS(500));
	}
}

static uint16_t getVoltage()
{
	uint16_t voltage_ = 0;
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage_ = voltage;
	osMutexRelease(mutexVoltageHandle);
	return voltage_;
}

static uint16_t getLux()
{
	uint16_t lux_ = 0;
	osMutexAcquire(mutexLuxHandle, osWaitForever);
	lux_ = lux;
	osMutexRelease(mutexLuxHandle);
	return lux_;
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

static void startTransmissionOfDisplayData()
{
	HAL_TIM_Base_Start_IT(&htim7);
}

static void waitForEndOfTransmission()
{
	osSemaphoreAcquire(semaphoreDisplayHandle, osWaitForever);
}

void transmitDisplayData() //TODO Переименовать в interrupt handler
{
	static TransmissionState transmissionState = CLK_RISING_EDGE;
	static int8_t byteIndex = DISPLAY_DATA_SIZE - 1;
	static uint8_t bitIndex = 0;

	switch(transmissionState)
	{
		case CLK_RISING_EDGE:
		default:

			uint8_t byteToTransmit = displayData[byteIndex];
			uint8_t bitToTransmit = (byteToTransmit >> bitIndex) & 0x1;
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

			transmissionState = CLK_FALLING_EDGE;

			break;


		case CLK_FALLING_EDGE:

			HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

			++bitIndex;
			if(bitIndex > 7)
			{
				bitIndex = 0;
				--byteIndex;
			}
			if(byteIndex >= 0)
			{
				transmissionState = CLK_RISING_EDGE;
			}
			else
			{
				byteIndex = DISPLAY_DATA_SIZE - 1;
				transmissionState = STROBE_RISING_EDGE;
			}

			break;


		case STROBE_RISING_EDGE:

			HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin,
							  GPIO_PIN_SET);

			transmissionState = STROBE_FALLING_EDGE;

			break;


		case STROBE_FALLING_EDGE:

			HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin,
							  GPIO_PIN_RESET);

			HAL_TIM_Base_Stop_IT(&htim7);
			transmissionState = CLK_RISING_EDGE;
			osSemaphoreRelease(semaphoreDisplayHandle);

			break;
	}
}
