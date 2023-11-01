#include <stdbool.h>
#include "cmsis_os.h"
#include "tim.h"
#include "voltage.h"
#include "lux.h"


#define DISPLAY_DATA_SIZE 9u /**< Длина массива данных дисплея. */


/**
  Состояние передачи данных на дисплей.
 */
typedef enum
{
	DISPLAY_TX_STATE_CLK_RISING_EDGE,	 /**< Передний фронт сигнала тактирования CLK. */
	DISPLAY_TX_STATE_CLK_FALLING_EDGE,	 /**< Задний фронт сигнала тактирования CLK. */
	DISPLAY_TX_STATE_STROBE_RISING_EDGE, /**< Передний фронт сигнала STROBE. */
	DISPLAY_TX_STATE_STROBE_FALLING_EDGE /**< Задний фронт сигнала STROBE. */
} DisplayTxState;


extern osSemaphoreId_t semaphoreDisplayHandle; /**< Семафор для сигнализации окончания передачи данных на дисплей. */
const uint32_t TASK_DISPLAY_PERIOD = 100u;	   /**< Период запуска задачи передачи данных на дисплей, мс */

volatile uint8_t displayData[DISPLAY_DATA_SIZE]; /**< Массив данных дисплея. */
const uint8_t DISPLAY1_FIRST_DIGIT	= 6u;		 /**< Индекс первой цифры первого индикатора в массиве данных дисплея. */
const uint8_t DISPLAY1_SECOND_DIGIT	= 7u;		 /**< Индекс второй цифры первого индикатора в массиве данных дисплея. */
const uint8_t DISPLAY1_THIRD_DIGIT	= 5u;		 /**< Индекс третьей цифры первого индикатора в массиве данных дисплея. */
const uint8_t DISPLAY2_FIRST_DIGIT	= 1u;		 /**< Индекс первой цифры второго индикатора в массиве данных дисплея. */
const uint8_t DISPLAY2_SECOND_DIGIT	= 2u;		 /**< Индекс второй цифры второго индикатора в массиве данных дисплея. */
const uint8_t DISPLAY2_THIRD_DIGIT	= 0u;		 /**< Индекс третьей цифры второго индикатора в массиве данных дисплея. */
const uint8_t LEDS_LEFT				= 3u;		 /**< Индекс левой группы светодиодов в массиве данных дисплея. */
const uint8_t LEDS_RIGHT			= 8u;		 /**< Индекс правой группы светодиодов в массиве данных дисплея. */
const uint8_t MIX_LEDS				= 4u;		 /**< Индекс отдельных светодиодов в массиве данных дисплея. */

const uint8_t DISPLAY_EMPTY			= 0xFF; /**< Значение пустого индикатора. */
const uint8_t DISPLAY_DIGIT_0		= 0x88; /**< Значение "0" на индикаторе. */
const uint8_t DISPLAY_DIGIT_1		= 0xEE; /**< Значение "1" на индикаторе. */
const uint8_t DISPLAY_DIGIT_2		= 0x94; /**< Значение "2" на индикаторе. */
const uint8_t DISPLAY_DIGIT_3		= 0xC4; /**< Значение "3" на индикаторе. */
const uint8_t DISPLAY_DIGIT_4		= 0xE2; /**< Значение "4" на индикаторе. */
const uint8_t DISPLAY_DIGIT_5		= 0xC1; /**< Значение "5" на индикаторе. */
const uint8_t DISPLAY_DIGIT_6		= 0x81; /**< Значение "6" на индикаторе. */
const uint8_t DISPLAY_DIGIT_7		= 0xEC; /**< Значение "7" на индикаторе. */
const uint8_t DISPLAY_DIGIT_8		= 0x80; /**< Значение "8" на индикаторе. */
const uint8_t DISPLAY_DIGIT_9		= 0xC0; /**< Значение "9" на индикаторе. */
const uint8_t DISPLAY_DIGIT_0_DP	= 0x08; /**< Значение "0." на индикаторе. */
const uint8_t DISPLAY_DIGIT_1_DP	= 0x6E; /**< Значение "1." на индикаторе. */
const uint8_t DISPLAY_DIGIT_2_DP	= 0x14; /**< Значение "2." на индикаторе. */
const uint8_t DISPLAY_DIGIT_3_DP	= 0x44; /**< Значение "3." на индикаторе. */
const uint8_t DISPLAY_DIGIT_4_DP	= 0x62; /**< Значение "4." на индикаторе. */
const uint8_t DISPLAY_DIGIT_5_DP	= 0x41; /**< Значение "5." на индикаторе. */
const uint8_t DISPLAY_DIGIT_6_DP	= 0x01; /**< Значение "6." на индикаторе. */
const uint8_t DISPLAY_DIGIT_7_DP	= 0x6C; /**< Значение "7." на индикаторе. */
const uint8_t DISPLAY_DIGIT_8_DP	= 0x00; /**< Значение "8." на индикаторе. */
const uint8_t DISPLAY_DIGIT_9_DP	= 0x40; /**< Значение "9." на индикаторе. */
const uint8_t DISPLAY_FULL			= 0x00; /**< Значение полностью горящего индикатора. */
const uint8_t LEDS_OFF				= 0xFF; /**< Значение выключенной группы светодиодов. */
const uint8_t MIX_LEDS_OFF			= 0x71; /**< Значение выключенных отдельных светодиодов. */


static void displayInit();
static void convertVoltageToDisplayData(uint32_t voltage);
static void convertLuxToDisplayData(uint16_t lux);
static void convertParameterToDisplayData(uint32_t parameter,
								   	   	  uint8_t displayFirstDigitIndex,
										  uint8_t displaySecondDigitIndex,
										  uint8_t displayThirdDigitIndex);
static uint8_t convertDigitToDisplayData(uint8_t digit, bool dotPoint);
static void startTransmissionOfDisplayData();
static void waitForEndOfTransmission();


/**
  @brief Задача RTOS передачи данных на дисплей.

  Получает напряжение батареи и освещённость. \n
  Преобразует напряжение батареи и освещённость в данные для передачи на дисплей. \n
  Передаёт данные на дисплей. \n
  Ожидает окончания передачи данных. \n
  Период запуска задачи составляет 100 мс.
 */
void taskDisplayFunction(void *argument)
{
	displayInit();

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		uint32_t voltage_ = getVoltage();
		uint16_t lux_ = getLux();
		convertVoltageToDisplayData(voltage_);
		convertLuxToDisplayData(lux_);
		startTransmissionOfDisplayData();
		waitForEndOfTransmission();

		osDelayUntil(tick + pdMS_TO_TICKS(TASK_DISPLAY_PERIOD));
	}
}

/**
  @brief Инициализирует дисплей пустыми значениями.
 */
static void displayInit()
{
	displayData[DISPLAY1_FIRST_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY1_SECOND_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY1_THIRD_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_FIRST_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_SECOND_DIGIT]	= DISPLAY_EMPTY;
	displayData[DISPLAY2_THIRD_DIGIT]	= DISPLAY_EMPTY;
	displayData[LEDS_LEFT]				= LEDS_OFF;
	displayData[LEDS_RIGHT]				= LEDS_OFF;
	displayData[MIX_LEDS]				= MIX_LEDS_OFF;

	osSemaphoreAcquire(semaphoreDisplayHandle, osWaitForever);

	startTransmissionOfDisplayData();
	waitForEndOfTransmission();
}

/**
  @brief Преобразует напряжение батареи в данные для передачи на дисплей.
  @param voltage Напряжение батареи, мВ
 */
static void convertVoltageToDisplayData(uint32_t voltage)
{
	convertParameterToDisplayData(voltage,
						   	   	  DISPLAY1_FIRST_DIGIT,
								  DISPLAY1_SECOND_DIGIT,
								  DISPLAY1_THIRD_DIGIT);
}

/**
  @brief Преобразует освещённость в данные для передачи на дисплей.
  @param lux Освещённость, лк
 */
static void convertLuxToDisplayData(uint16_t lux)
{
	convertParameterToDisplayData(lux,
						   	   	  DISPLAY2_FIRST_DIGIT,
								  DISPLAY2_SECOND_DIGIT,
								  DISPLAY2_THIRD_DIGIT);
}

/**
  @brief Преобразует параметр в десятичном виде в данные для передачи на дисплей.
  @param parameter Параметр в десятичном виде.
  @param displayFirstDigitIndex Индекс первой цифры индикатора в массиве данных дисплея.
  @param displaySecondDigitIndex Индекс второй цифры индикатора в массиве данных дисплея.
  @param displayThirdDigitIndex Индекс третьей цифры индикатора в массиве данных дисплея.
 */
static void convertParameterToDisplayData(uint32_t parameter,
								   	   	  uint8_t displayFirstDigitIndex,
										  uint8_t displaySecondDigitIndex,
										  uint8_t displayThirdDigitIndex)
{
	uint8_t firstDigitDisplayData  = DISPLAY_EMPTY;
	uint8_t secondDigitDisplayData = DISPLAY_EMPTY;
	uint8_t thirdDigitDisplayData  = DISPLAY_EMPTY;

	uint8_t firstDigit  = 0;
	uint8_t secondDigit = 0;
	uint8_t thirdDigit  = 0;

	if(parameter <= 999)
	{
		thirdDigit = parameter%10;
		thirdDigitDisplayData
		= convertDigitToDisplayData(thirdDigit, false);

		if((parameter/10) != 0)
		{
			parameter = parameter/10;
			secondDigit = parameter%10;
			secondDigitDisplayData
			= convertDigitToDisplayData(secondDigit, false);

			if((parameter/10) != 0)
			{
				parameter = parameter/10;
				firstDigit = parameter%10;
				firstDigitDisplayData
				= convertDigitToDisplayData(firstDigit, false);
			}
		}
	}
	else
	{
		uint32_t integerPart = parameter/1000;
		uint32_t remainder = parameter%1000;

		while((remainder/10) != 0)
		{
			remainder = remainder/10;
		}
		thirdDigit = remainder%10;
		thirdDigitDisplayData
			= convertDigitToDisplayData(thirdDigit, false);

		secondDigit = integerPart%10;
		secondDigitDisplayData
				= convertDigitToDisplayData(secondDigit, true);

		if((integerPart/10) != 0)
		{
			integerPart = integerPart/10;
			firstDigit = integerPart%10;
			firstDigitDisplayData
			= convertDigitToDisplayData(firstDigit, false);
		}
	}

	displayData[displayFirstDigitIndex]  = firstDigitDisplayData;
	displayData[displaySecondDigitIndex] = secondDigitDisplayData;
	displayData[displayThirdDigitIndex]  = thirdDigitDisplayData;
}

/**
  @brief Преобразует цифру в десятичном виде в данные для передачи на дисплей.
  @param digit Цифра в десятичном виде.
  @param dotPoint Точка. \n
  true - С точкой. \n
  false - Без точки. \n
  @return Данные для передачи на дисплей.
 */
static uint8_t convertDigitToDisplayData(uint8_t digit, bool dotPoint)
{
	uint8_t data = DISPLAY_EMPTY;

	if(!dotPoint)
	{
		switch(digit)
		{
			case 0:
			default:
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
				data = DISPLAY_DIGIT_9;
				break;
		}
	}
	else
	{
		switch(digit)
		{
			case 0:
			default:
				data = DISPLAY_DIGIT_0_DP;
				break;
			case 1:
				data = DISPLAY_DIGIT_1_DP;
				break;
			case 2:
				data = DISPLAY_DIGIT_2_DP;
				break;
			case 3:
				data = DISPLAY_DIGIT_3_DP;
				break;
			case 4:
				data = DISPLAY_DIGIT_4_DP;
				break;
			case 5:
				data = DISPLAY_DIGIT_5_DP;
				break;
			case 6:
				data = DISPLAY_DIGIT_6_DP;
				break;
			case 7:
				data = DISPLAY_DIGIT_7_DP;
				break;
			case 8:
				data = DISPLAY_DIGIT_8_DP;
				break;
			case 9:
				data = DISPLAY_DIGIT_9_DP;
				break;
		}
	}

	return data;
}

/**
  @brief Запускает передачу данных на дисплей.
 */
static void startTransmissionOfDisplayData()
{
	HAL_TIM_Base_Start_IT(&htim7);
}

/**
  @brief Ожидает окончания передачи данных.
 */
static void waitForEndOfTransmission()
{
	osSemaphoreAcquire(semaphoreDisplayHandle, osWaitForever);
}

/**
  @brief Передаёт данные на дисплей. Запускается по прерыванию от таймера.
 */
void displayInterruptHandler()
{
	static DisplayTxState displayTxState = DISPLAY_TX_STATE_CLK_RISING_EDGE;
	static int8_t byteIndex = DISPLAY_DATA_SIZE - 1;
	static uint8_t bitIndex = 0;

	switch(displayTxState)
	{
		case DISPLAY_TX_STATE_CLK_RISING_EDGE:
		default:
		{
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

			displayTxState = DISPLAY_TX_STATE_CLK_FALLING_EDGE;

			break;
		}

		case DISPLAY_TX_STATE_CLK_FALLING_EDGE:
		{
			HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

			++bitIndex;
			if(bitIndex > 7)
			{
				bitIndex = 0;
				--byteIndex;
			}
			if(byteIndex >= 0)
			{
				displayTxState = DISPLAY_TX_STATE_CLK_RISING_EDGE;
			}
			else
			{
				byteIndex = DISPLAY_DATA_SIZE - 1;
				displayTxState = DISPLAY_TX_STATE_STROBE_RISING_EDGE;
			}

			break;
		}

		case DISPLAY_TX_STATE_STROBE_RISING_EDGE:
		{
			HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin,
							  GPIO_PIN_SET);

			displayTxState = DISPLAY_TX_STATE_STROBE_FALLING_EDGE;

			break;
		}

		case DISPLAY_TX_STATE_STROBE_FALLING_EDGE:
		{
			HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin,
							  GPIO_PIN_RESET);

			displayTxState = DISPLAY_TX_STATE_CLK_RISING_EDGE;

			HAL_TIM_Base_Stop_IT(&htim7);
			osSemaphoreRelease(semaphoreDisplayHandle);

			break;
		}
	}
}
