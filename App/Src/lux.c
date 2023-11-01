#include <stdbool.h>
#include "cmsis_os.h"
#include "i2c.h"


#define LTR_REGISTERS_SIZE 9u /**< Длина массива регистров датчика освещённости. */


/**
  Режим работы датчика освещённости.
 */
typedef enum
{
	LTR_MODE_STANDBY = 0x0, /**< Датчик освещённости не работает. */
	LTR_MODE_ACTIVE	 = 0x1  /**< Датчик освещённости работает. */
} LtrMode;

/**
  Перезагрузка датчика освещённости.
 */
typedef enum
{
	LTR_RESET_OFF = 0x0, /**< Перезагрузка датчика освещённости не производится. */
	LTR_RESET_ON  = 0x1  /**< Перезагрузка датчика освещённости. */
} LtrReset;

/**
  Усиление датчика освещённости.
 */
typedef enum
{
	LTR_GAIN_1X  = 0x0, /**< Усиление датчика освещённости = 1. Диапазон измерения от 1 до 64000 люксов. */
	LTR_GAIN_2X  = 0x1, /**< Усиление датчика освещённости = 2. Диапазон измерения от 0,5 до 32000 люксов. */
	LTR_GAIN_4X  = 0x2, /**< Усиление датчика освещённости = 4. Диапазон измерения от 0,25 до 16000 люксов. */
	LTR_GAIN_8X  = 0x3, /**< Усиление датчика освещённости = 8. Диапазон измерения от 0,125 до 8000 люксов. */
	LTR_GAIN_48X = 0x6, /**< Усиление датчика освещённости = 48. Диапазон измерения от 0,02 до 1300 люксов. */
	LTR_GAIN_96X = 0x7  /**< Усиление датчика освещённости = 96. Диапазон измерения от 0,01 до 600 люксов. */
} LtrGain;

/**
  Период измерения датчика освещённости.
 */
typedef enum
{
	LTR_MEASUREMENT_RATE_50MS	= 0x0, /**< Период измерения датчика освещённости = 50 мс */
	LTR_MEASUREMENT_RATE_100MS	= 0x1, /**< Период измерения датчика освещённости = 100 мс */
	LTR_MEASUREMENT_RATE_200MS	= 0x2, /**< Период измерения датчика освещённости = 200 мс */
	LTR_MEASUREMENT_RATE_500MS	= 0x3, /**< Период измерения датчика освещённости = 500 мс */
	LTR_MEASUREMENT_RATE_1000MS = 0x4, /**< Период измерения датчика освещённости = 1000 мс */
	LTR_MEASUREMENT_RATE_2000MS = 0x5  /**< Период измерения датчика освещённости = 2000 мс */
} LtrMeasurementRate;

/**
  Время интеграции датчика освещённости.
 */
typedef enum
{
	LTR_INTEGRATION_TIME_100MS	= 0x0, /**< Время интеграции датчика освещённости = 100 мс */
	LTR_INTEGRATION_TIME_50MS	= 0x1, /**< Время интеграции датчика освещённости = 50 мс */
	LTR_INTEGRATION_TIME_200MS	= 0x2, /**< Время интеграции датчика освещённости = 200 мс */
	LTR_INTEGRATION_TIME_400MS	= 0x3, /**< Время интеграции датчика освещённости = 400 мс */
	LTR_INTEGRATIONG_TIME_150MS = 0x4, /**< Время интеграции датчика освещённости = 150 мс */
	LTR_INTEGRATION_TIME_250MS	= 0x5, /**< Время интеграции датчика освещённости = 250 мс */
	LTR_INTEGRATION_TIME_300MS	= 0x6, /**< Время интеграции датчика освещённости = 300 мс */
	LTR_INTEGRATION_TIME_350MS	= 0x7  /**< Время интеграции датчика освещённости = 350 мс */
} LtrIntegrationTime;

/**
  Статус измерения освещённости.
 */
typedef enum
{
	LTR_DATA_STATUS_OLD = 0x0, /**< Измерение освещённости не закончено. */
	LTR_DATA_STATUS_NEW = 0x1  /**< Измерение освещённости закончено. */
} LtrDataStatus;

/**
  Усиление датчика освещённости.
 */
typedef enum
{
	LTR_DATA_GAIN_RANGE_1X	= 0x0, /**< Усиление датчика освещённости = 1. Диапазон измерения от 1 до 64000 люксов. */
	LTR_DATA_GAIN_RANGE_2X	= 0x1, /**< Усиление датчика освещённости = 2. Диапазон измерения от 0,5 до 32000 люксов. */
	LTR_DATA_GAIN_RANGE_4X	= 0x2, /**< Усиление датчика освещённости = 4. Диапазон измерения от 0,25 до 16000 люксов. */
	LTR_DATA_GAIN_RANGE_8X	= 0x3, /**< Усиление датчика освещённости = 8. Диапазон измерения от 0,125 до 8000 люксов. */
	LTR_DATA_GAIN_RANGE_48X = 0x6, /**< Усиление датчика освещённости = 48. Диапазон измерения от 0,02 до 1300 люксов. */
	LTR_DATA_GAIN_RANGE_96X = 0x7  /**< Усиление датчика освещённости = 96. Диапазон измерения от 0,01 до 600 люксов. */
} LtrDataGainRange;

/**
  Достоверность данных датчика освещённости.
 */
typedef enum
{
	LTR_DATA_VALIDITY_VALID	  = 0x0, /**< Данные датчика освещённости достоверны. */
	LTR_DATA_VALIDITY_INVALID = 0x1  /**< Данные датчика освещённости не достоверны. */
} LtrDataValidity;


volatile uint16_t lux = 0u;			   /**< Освещённость, лк */
extern osMutexId_t mutexI2cHandle;	   /**< Мьютекс для доступа к I2C. */
extern osMutexId_t mutexLuxHandle;	   /**< Мьютекс для доступа к освещённости. */
const uint32_t TASK_LUX_PERIOD = 100u; /**< Период запуска задачи чтения освещённости, мс */

uint8_t ltrRegisters[LTR_REGISTERS_SIZE];	   /**< Массив регистров датчика освещённости. */
const uint8_t LTR_CONTROL_INDEX			 = 0u; /**< Индекс регистра CONTROL в массиве регистров датчика освещённости. */
const uint8_t LTR_MEASUREMENT_RATE_INDEX = 1u; /**< Индекс регистра MEASUREMENT_RATE в массиве регистров датчика освещённости. */
const uint8_t LTR_PART_ID_INDEX			 = 2u; /**< Индекс регистра PART_ID в массиве регистров датчика освещённости. */
const uint8_t LTR_MANUFACTURER_ID_INDEX	 = 3u; /**< Индекс регистра MANUFACTURER_ID в массиве регистров датчика освещённости. */
const uint8_t LTR_CHANNEL1_LOW_INDEX	 = 4u; /**< Индекс регистра CHANNEL1_LOW в массиве регистров датчика освещённости. */
const uint8_t LTR_CHANNEL1_HIGH_INDEX	 = 5u; /**< Индекс регистра CHANNEL1_HIGH в массиве регистров датчика освещённости. */
const uint8_t LTR_CHANNEL0_LOW_INDEX	 = 6u; /**< Индекс регистра CHANNEL0_LOW в массиве регистров датчика освещённости. */
const uint8_t LTR_CHANNEL0_HIGH_INDEX	 = 7u; /**< Индекс регистра CHANNEL0_HIGH в массиве регистров датчика освещённости. */
const uint8_t LTR_STATUS_INDEX	   	 	 = 8u; /**< Индекс регистра STATUS в массиве регистров датчика освещённости. */

const uint8_t LTR_ADDRESS		 		   = 0x29; /**< Адрес датчика отсвещённости. */
const uint8_t LTR_CONTROL_ADDRESS		   = 0x80; /**< Адрес регистра CONTROL датчика освещённости. */
const uint8_t LTR_MEASUREMENT_RATE_ADDRESS = 0x85; /**< Адрес регистра MEASUREMENT_RATE датчика освещённости. */
const uint8_t LTR_PART_ID_ADDRESS	 	   = 0x86; /**< Адрес регистра PART_ID датчика освещённости. */
const uint8_t LTR_MANUFACTURER_ID_ADDRESS  = 0x87; /**< Адрес регистра MANUFACTURER_ID датчика освещённости. */
const uint8_t LTR_CHANNEL1_LOW_ADDRESS	   = 0x88; /**< Адрес регистра CHANNEL1_LOW датчика освещённости. */
const uint8_t LTR_CHANNEL1_HIGH_ADDRESS	   = 0x89; /**< Адрес регистра CHANNEL1_HIGH датчика освещённости. */
const uint8_t LTR_CHANNEL0_LOW_ADDRESS	   = 0x8A; /**< Адрес регистра CHANNEL0_LOW датчика освещённости. */
const uint8_t LTR_CHANNEL0_HIGH_ADDRESS	   = 0x8B; /**< Адрес регистра CHANNEL0_HIGH датчика освещённости. */
const uint8_t LTR_STATUS_ADDRESS		   = 0x8C; /**< Адрес регистра STATUS датчика освещённости. */

const uint8_t LTR_PART_NUMBER_ID  = 0x0A; /**< Серийный номер датчика освещённости. */
const uint8_t LTR_REVISION_ID	  = 0x00; /**< Номер ревизии датчика освещённости. */
const uint8_t LTR_MANUFACTURER_ID = 0x05; /**< Производственный номер датчика освещённости. */

const uint8_t LTR_MODE_OFFSET			  = 0u; /**< Сдвиг поля MODE в регистре CONTROL датчика освещённости. */
const uint8_t LTR_RESET_OFFSET		  	  = 1u; /**< Сдвиг поля RESET в регистре CONTROL датчика освещённости. */
const uint8_t LTR_GAIN_OFFSET			  = 2u; /**< Сдвиг поля GAIN в регистре CONTROL датчика освещённости. */
const uint8_t LTR_MEASUREMENT_RATE_OFFSET = 0u; /**< Сдвиг поля MEASUREMENT_RATE в регистре MEASUREMENT_RATE датчика освещённости. */
const uint8_t LTR_INTEGRATION_TIME_OFFSET = 3u; /**< Сдвиг поля INTEGRATION_TIME в регистре MEASUREMENT_RATE датчика освещённости. */
const uint8_t LTR_REVISION_ID_OFFSET	  = 0u; /**< Сдвиг поля REVISION_ID в регистре PART_ID датчика освещённости. */
const uint8_t LTR_PART_NUMBER_ID_OFFSET	  = 4u; /**< Сдвиг поля PART_NUMBER_ID в регистре PART_ID датчика освещённости. */
const uint8_t LTR_DATA_STATUS_OFFSET	  = 2u; /**< Сдвиг поля DATA_STATUS в регистре STATUS датчика освещённости. */
const uint8_t LTR_DATA_GAIN_RANGE_OFFSET  = 4u; /**< Сдвиг поля DATA_GAIN_RANGE в регистре STATUS датчика освещённости. */
const uint8_t LTR_DATA_VALIDITY_OFFSET	  = 7u; /**< Сдвиг поля DATA_VALIDITY в регистре STATUS датчика освещённости. */

const uint8_t LTR_MODE_MASK				  = 0x01; /**< Маска для чтения поля MODE в регистре CONTROL датчика освещённости. */
const uint8_t LTR_RESET_MASK			  = 0x02; /**< Маска для чтения поля RESET в регистре CONTROL датчика освещённости. */
const uint8_t LTR_GAIN_MASK				  = 0x1C; /**< Маска для чтения поля GAIN в регистре CONTROL датчика освещённости. */
const uint8_t LTR_MEASUREMENT_RATE_MASK	  = 0x07; /**< Маска для чтения поля MEASUREMENT_RATE в регистре MEASUREMENT_RATE датчика освещённости. */
const uint8_t LTR_INTEGRATION_TIME_MASK	  = 0x38; /**< Маска для чтения поля INTEGRATION_TIME в регистре MEASUREMENT_RATE датчика освещённости. */
const uint8_t LTR_REVISION_ID_MASK		  = 0x0F; /**< Маска для чтения поля REVISION_ID в регистре PART_ID датчика освещённости. */
const uint8_t LTR_PART_NUMBER_ID_MASK	  = 0xF0; /**< Маска для чтения поля PART_NUMBER_ID в регистре PART_ID датчика освещённости. */
const uint8_t LTR_DATA_STATUS_MASK		  = 0x04; /**< Маска для чтения поля DATA_STATUS в регистре STATUS датчика освещённости. */
const uint8_t LTR_DATA_GAIN_RANGE_MASK	  = 0x70; /**< Маска для чтения поля DATA_GAIN_RANGE в регистре STATUS датчика освещённости. */
const uint8_t LTR_DATA_VALIDITY_MASK	  = 0x80; /**< Маска для чтения поля DATA_VALIDITY в регистре STATUS датчика освещённости. */

const uint32_t LTR_TIME_TO_POWER_UP = 100u; /**< Время ожидания до полного запуска датчика освещённости, мс */
const uint32_t LTR_TIME_TO_WAKE_UP	= 10u;  /**< Время ожидания до начала работы после запуска датчика освещённости, мс */


static void luxInit();

static void setDefaultLtrRegisterValues();
static void setLtrControl(LtrMode ltrMode,
						  LtrReset ltrReset,
						  LtrGain ltrGain);
static void setLtrMeasurementRate(LtrMeasurementRate ltrMeasurementRate,
								  LtrIntegrationTime ltrIntegrationTime);
static void setLtrPartId();
static void setLtrManufacturerId();
static void setLtrChannel1Low(uint8_t channel1Low);
static void setLtrChannel1High(uint8_t channel1High);
static void setLtrChannel0Low(uint8_t channel0Low);
static void setLtrChannel0High(uint8_t channel0High);
static void setLtrStatus(LtrDataStatus ltrDataStatus,
						 LtrDataGainRange ltrDataGainRange,
						 LtrDataValidity ltrDataValidity);

static LtrMode getLtrMode();
static LtrReset getLtrReset();
static LtrGain getLtrGain();
static LtrMeasurementRate getLtrMeasurementRate();
static LtrIntegrationTime getLtrIntegrationTime();
static uint8_t getLtrRevisionId();
static uint8_t getLtrPartNumberId();
static uint8_t getLtrManufacturerId();
static uint8_t getLtrChannel1Low();
static uint8_t getLtrChannel1High();
static uint8_t getLtrChannel0Low();
static uint8_t getLtrChannel0High();
static LtrDataStatus getLtrDataStatus();
static LtrDataGainRange getLtrDataGainRange();
static LtrDataValidity getLtrDataValidity();

static bool isNewLuxDataAvailable();
static void readLuxData();
static uint16_t calculateLux();
static void setLux(uint16_t lux_);


/**
  @brief Задача RTOS чтения освещённости из датчика освещённости.

  Проверяет готовность новых данных для чтения. \n
  В случае готовности читает новые данные в необработанном виде. \n
  Преобразует полученные данные в освещённость в люксах. \n
  Записывает освещённость в глобальную переменную. \n
  Период запуска задачи составляет 100 мс.
 */
void taskLuxFunction(void *argument)
{
	luxInit();

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		bool newLuxDataAvailable = isNewLuxDataAvailable();
		if(newLuxDataAvailable == true)
		{
			readLuxData();
			uint16_t lux_ = calculateLux();
			setLux(lux_);
		}

		osDelayUntil(tick + pdMS_TO_TICKS(TASK_LUX_PERIOD));
	}
}

/**
  @brief Инициализирует работу с датчиком освещённости.

  Ожидает время до полного запуска датчика освещённости. \n
  Проверяет связь по I2C. \n
  Настраивает и запускает работу датчика освещённости. \n
  Ожидает время до начала работы датчика освещённости.
 */
static void luxInit()
{
	osDelay(pdMS_TO_TICKS(LTR_TIME_TO_POWER_UP));

	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_IsDeviceReady(&hi2c1, LTR_ADDRESS << 1, 1, 1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	setDefaultLtrRegisterValues();
	setLtrControl(LTR_MODE_ACTIVE,
				  LTR_RESET_OFF,
				  LTR_GAIN_1X);
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Write
							(&hi2c1,
							 LTR_ADDRESS << 1,
							 LTR_CONTROL_ADDRESS, I2C_MEMADD_SIZE_8BIT,
							 ltrRegisters + LTR_CONTROL_INDEX, 1,
							 1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	setLtrMeasurementRate(LTR_MEASUREMENT_RATE_100MS,
						  LTR_INTEGRATION_TIME_100MS);
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Write
							(&hi2c1,
							LTR_ADDRESS << 1,
							LTR_MEASUREMENT_RATE_ADDRESS, I2C_MEMADD_SIZE_8BIT,
							ltrRegisters + LTR_MEASUREMENT_RATE_INDEX, 1,
							1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	osDelay(pdMS_TO_TICKS(LTR_TIME_TO_WAKE_UP));
}

/**
  @brief Записывает значения по умолчанию в массив регистров датчика освещённости.
 */
static void setDefaultLtrRegisterValues()
{
	setLtrControl(LTR_MODE_STANDBY,
				  LTR_RESET_OFF,
				  LTR_GAIN_1X);
	setLtrMeasurementRate(LTR_MEASUREMENT_RATE_500MS,
						  LTR_INTEGRATION_TIME_100MS);
	setLtrPartId();
	setLtrManufacturerId();
	setLtrChannel1Low(0);
	setLtrChannel1High(0);
	setLtrChannel0Low(0);
	setLtrChannel0High(0);
	setLtrStatus(LTR_DATA_STATUS_OLD,
				 LTR_DATA_GAIN_RANGE_1X,
				 LTR_DATA_VALIDITY_VALID);
}

/**
  @brief Записывает значение регистра CONTROL в массив регистров датчика освещённости.
  @param ltrMode Режим работы датчика освещённости.
  @param ltrReset Перезагрузка датчика освещённости.
  @param ltrGain Усиление датчика освещённости.
 */
static void setLtrControl(LtrMode ltrMode,
						  LtrReset ltrReset,
						  LtrGain ltrGain)
{
	uint8_t reg = 0;
	reg |= (ltrMode << LTR_MODE_OFFSET);
	reg |= (ltrReset << LTR_RESET_OFFSET);
	reg |= (ltrGain << LTR_GAIN_OFFSET);
	ltrRegisters[LTR_CONTROL_INDEX] = reg;
}

/**
  @brief Записывает значение регистра MEASUREMENT_RATE в массив регистров датчика освещённости.
  @param ltrMeasurementRate Период измерения датчика освещённости.
  @param ltrIntegrationTime Время интеграции датчика освещённости.
 */
static void setLtrMeasurementRate(LtrMeasurementRate ltrMeasurementRate,
								  LtrIntegrationTime ltrIntegrationTime)
{
	uint8_t reg = 0;
	reg |= (ltrMeasurementRate << LTR_MEASUREMENT_RATE_OFFSET);
	reg |= (ltrIntegrationTime << LTR_INTEGRATION_TIME_OFFSET);
	ltrRegisters[LTR_MEASUREMENT_RATE_INDEX] = reg;
}

/**
  @brief Записывает значение регистра PART_ID в массив регистров датчика освещённости.
 */
static void setLtrPartId()
{
	uint8_t reg = 0;
	reg |= (LTR_REVISION_ID << LTR_REVISION_ID_OFFSET);
	reg |= (LTR_PART_NUMBER_ID << LTR_PART_NUMBER_ID_OFFSET);
	ltrRegisters[LTR_PART_ID_INDEX] = reg;
}

/**
  @brief Записывает значение регистра MANUFACTURER_ID в массив регистров датчика освещённости.
 */
static void setLtrManufacturerId()
{
	ltrRegisters[LTR_MANUFACTURER_ID_INDEX] = LTR_MANUFACTURER_ID;
}

/**
  @brief Записывает значение регистра CHANNEL1_LOW в массив регистров датчика освещённости.
  @param channel1Low Младший байт данных канала 1.
 */
static void setLtrChannel1Low(uint8_t channel1Low)
{
	ltrRegisters[LTR_CHANNEL1_LOW_INDEX] = channel1Low;
}

/**
  @brief Записывает значение регистра CHANNEL1_HIGH в массив регистров датчика освещённости.
  @param channel1High Старший байт данных канала 1.
 */
static void setLtrChannel1High(uint8_t channel1High)
{
	ltrRegisters[LTR_CHANNEL1_HIGH_INDEX] = channel1High;
}

/**
  @brief Записывает значение регистра CHANNEL0_LOW в массив регистров датчика освещённости.
  @param channel0Low Младший байт данных канала 0.
 */
static void setLtrChannel0Low(uint8_t channel0Low)
{
	ltrRegisters[LTR_CHANNEL0_LOW_INDEX] = channel0Low;
}

/**
  @brief Записывает значение регистра CHANNEL0_HIGH в массив регистров датчика освещённости.
  @param channel0High Старший байт данных канала 0.
 */
static void setLtrChannel0High(uint8_t channel0High)
{
	ltrRegisters[LTR_CHANNEL0_HIGH_INDEX] = channel0High;
}

/**
  @brief Записывает значение регистра STATUS в массив регистров датчика освещённости.
  @param ltrDataStatus Статус измерения освещённости.
  @param ltrDataGainRange Усиление датчика освещённости.
  @param ltrDataValidity Достоверность данных датчика освещённости.
 */
static void setLtrStatus(LtrDataStatus ltrDataStatus,
					     LtrDataGainRange ltrDataGainRange,
					     LtrDataValidity ltrDataValidity)
{
	uint8_t reg = 0;
	reg |= (ltrDataStatus << LTR_DATA_STATUS_OFFSET);
	reg |= (ltrDataGainRange << LTR_DATA_GAIN_RANGE_OFFSET);
	reg |= (ltrDataValidity << LTR_DATA_VALIDITY_OFFSET);
	ltrRegisters[LTR_STATUS_INDEX] = reg;
}

/**
  @brief Возвращает режим работы датчика освещённости из массива регистров датчика освещённости.
  @return Режим работы датчика освещённости.
 */
static LtrMode getLtrMode()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrMode ltrMode = (LtrMode)((reg & LTR_MODE_MASK) >> LTR_MODE_OFFSET);
	return ltrMode;
}

/**
  @brief Возвращает перезагрузку датчика освещённости из массива регистров датчика освещённости.
  @return Перезагрузка датчика освещённости.
 */
static LtrReset getLtrReset()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrReset ltrReset = (LtrReset)((reg & LTR_RESET_MASK) >> LTR_RESET_OFFSET);
	return ltrReset;
}

/**
  @brief Возвращает усиление датчика освещённости из массива регистров датчика освещённости.
  @return Усиление датчика освещённости.
 */
static LtrGain getLtrGain()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrGain ltrGain = (LtrGain)((reg & LTR_GAIN_MASK) >> LTR_GAIN_OFFSET);
	return ltrGain;
}

/**
  @brief Возвращает период измерения датчика освещённости из массива регистров датчика освещённости.
  @return Период измерения датчика освещённости.
 */
static LtrMeasurementRate getLtrMeasurementRate()
{
	uint8_t reg = ltrRegisters[LTR_MEASUREMENT_RATE_INDEX];
	LtrMeasurementRate ltrMeasurementRate = (LtrMeasurementRate)
			((reg & LTR_MEASUREMENT_RATE_MASK) >> LTR_MEASUREMENT_RATE_OFFSET);
	return ltrMeasurementRate;
}

/**
  @brief Возвращает время интеграции датчика освещённости из массива регистров датчика освещённости.
  @return Время интеграции датчика освещённости.
 */
static LtrIntegrationTime getLtrIntegrationTime()
{
	uint8_t reg = ltrRegisters[LTR_MEASUREMENT_RATE_INDEX];
	LtrIntegrationTime ltrIntegrationTime = (LtrIntegrationTime)
			((reg & LTR_INTEGRATION_TIME_MASK) >> LTR_INTEGRATION_TIME_OFFSET);
	return ltrIntegrationTime;
}

/**
  @brief Возвращает номер ревизии датчика освещённости из массива регистров датчика освещённости.
  @return Номер ревизии датчика освещённости.
 */
static uint8_t getLtrRevisionId()
{
	uint8_t reg = ltrRegisters[LTR_PART_ID_INDEX];
	uint8_t ltrRevisionId =
			(reg & LTR_REVISION_ID_MASK) >> LTR_REVISION_ID_OFFSET;
	return ltrRevisionId;
}


/**
  @brief Возвращает серийный номер датчика освещённости из массива регистров датчика освещённости.
  @return Серийный номер датчика освещённости.
 */
static uint8_t getLtrPartNumberId()
{
	uint8_t reg = ltrRegisters[LTR_PART_ID_INDEX];
	uint8_t ltrPartNumberId =
			(reg & LTR_PART_NUMBER_ID_MASK) >> LTR_PART_NUMBER_ID_OFFSET;
	return ltrPartNumberId;
}

/**
  @brief Возвращает производственный номер датчика освещённости из массива регистров датчика освещённости.
  @return Производственный номер датчика освещённости.
 */
static uint8_t getLtrManufacturerId()
{
	return ltrRegisters[LTR_MANUFACTURER_ID_INDEX];
}

/**
  @brief Возвращает младший байт данных канала 1 из массива регистров датчика освещённости.
  @return Младший байт данных канала 1.
 */
static uint8_t getLtrChannel1Low()
{
	return ltrRegisters[LTR_CHANNEL1_LOW_INDEX];
}

/**
  @brief Возвращает старший байт данных канала 1 из массива регистров датчика освещённости.
  @return Старший байт данных канала 1.
 */
static uint8_t getLtrChannel1High()
{
	return ltrRegisters[LTR_CHANNEL1_HIGH_INDEX];
}

/**
  @brief Возвращает младший байт данных канала 0 из массива регистров датчика освещённости.
  @return Младший байт данных канала 0.
 */
static uint8_t getLtrChannel0Low()
{
	return ltrRegisters[LTR_CHANNEL0_LOW_INDEX];
}

/**
  @brief Возвращает старший байт данных канала 0 из массива регистров датчика освещённости.
  @return Старший байт данных канала 0.
 */
static uint8_t getLtrChannel0High()
{
	return ltrRegisters[LTR_CHANNEL0_HIGH_INDEX];
}

/**
  @brief Возвращает статус измерения освещённости из массива регистров датчика освещённости.
  @return Статус измерения освещённости.
 */
static LtrDataStatus getLtrDataStatus()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataStatus ltrDataStatus = (LtrDataStatus)
			((reg & LTR_DATA_STATUS_MASK) >> LTR_DATA_STATUS_OFFSET);
	return ltrDataStatus;
}

/**
  @brief Возвращает усиление датчика освещённости из массива регистров датчика освещённости.
  @return Усиление датчика освещённости.
 */
static LtrDataGainRange getLtrDataGainRange()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataGainRange ltrDataGainRange = (LtrDataGainRange)
			((reg & LTR_DATA_GAIN_RANGE_MASK) >> LTR_DATA_GAIN_RANGE_OFFSET);
	return ltrDataGainRange;
}

/**
  @brief Возвращает достоверность данных датчика освещённости из массива регистров датчика освещённости.
  @return Достоверность данных датчика освещённости.
 */
static LtrDataValidity getLtrDataValidity()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataValidity ltrDataValidity = (LtrDataValidity)
			((reg & LTR_DATA_VALIDITY_MASK) >> LTR_DATA_VALIDITY_OFFSET);
	return ltrDataValidity;
}

/**
  @brief Проверяет готовность новых данных датчика освещённости для чтения.
  @return Готовность новых данных датчика освещённости для чтения. \n
  true - Новые данные датчика освещённости готовы для чтения. \n
  false - Новые данные датчика освещённости не готовы для чтения.
 */
static bool isNewLuxDataAvailable()
{
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Read
						  	 (&hi2c1,
							  LTR_ADDRESS << 1,
							  LTR_STATUS_ADDRESS, I2C_MEMADD_SIZE_8BIT,
							  ltrRegisters + LTR_STATUS_INDEX, 1,
							  1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	LtrDataStatus ltrDataStatus = getLtrDataStatus();
	LtrDataValidity ltrDataValidity = getLtrDataValidity();
	if((ltrDataStatus == LTR_DATA_STATUS_NEW) &&
	   (ltrDataValidity == LTR_DATA_VALIDITY_VALID))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
  @brief Читает новые данные датчика освещённости в необработанном виде.
 */
static void readLuxData()
{
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Read
						  	 (&hi2c1,
							  LTR_ADDRESS << 1,
							  LTR_CHANNEL1_LOW_ADDRESS, I2C_MEMADD_SIZE_8BIT,
							  ltrRegisters + LTR_CHANNEL1_LOW_INDEX, 4,
							  1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);
}

/**
  @brief Преобразует полученные данные из датчика освещённости в освещённость в люксах.
  @return Освещённость, лк
 */
static uint16_t calculateLux()
{
	uint16_t lux_ = 0;

	uint16_t channel0 = 0;
	uint8_t channel0High = getLtrChannel0High();
	uint8_t channel0Low = getLtrChannel0Low();
	channel0 = ((uint16_t)channel0High << 8) | (uint16_t)channel0Low;

	uint16_t channel1 = 0;
	uint8_t channel1High = getLtrChannel1High();
	uint8_t channel1Low = getLtrChannel1Low();
	channel1 = ((uint16_t)channel1High << 8) | (uint16_t)channel1Low;

	uint8_t gain = 1;
	LtrGain ltrGain = getLtrGain();
	switch(ltrGain)
	{
		case LTR_GAIN_1X:
		default:
			gain = 1;
			break;
		case LTR_GAIN_2X:
			gain = 2;
			break;
		case LTR_GAIN_4X:
			gain = 4;
			break;
		case LTR_GAIN_8X:
			gain = 8;
			break;
		case LTR_GAIN_48X:
			gain = 48;
			break;
		case LTR_GAIN_96X:
			gain = 96;
			break;
	}

	float integrationTime = 1.0f;
	LtrIntegrationTime ltrIntegrationTime = getLtrIntegrationTime();
	switch(ltrIntegrationTime)
	{
		case LTR_INTEGRATION_TIME_100MS:
		default:
			integrationTime = 1.0f;
			break;
		case LTR_INTEGRATION_TIME_50MS:
			integrationTime = 0.5f;
			break;
		case LTR_INTEGRATION_TIME_200MS:
			integrationTime = 2.0f;
			break;
		case LTR_INTEGRATION_TIME_400MS:
			integrationTime = 4.0f;
			break;
		case LTR_INTEGRATIONG_TIME_150MS:
			integrationTime = 1.5f;
			break;
		case LTR_INTEGRATION_TIME_250MS:
			integrationTime = 2.5f;
			break;
		case LTR_INTEGRATION_TIME_300MS:
			integrationTime = 3.0f;
			break;
		case LTR_INTEGRATION_TIME_350MS:
			integrationTime = 3.5f;
			break;
	}

	float ratio = (float)channel1/((float)channel0 + (float)channel1);
	if(ratio < 0.45f)
	{
		lux_ = (1.7743f*(float)channel0 + 1.1059*(float)channel1)
				/(float)gain/integrationTime;
	}
	else if((ratio < 0.64f) && (ratio >= 0.45f))
	{
		lux_ = (4.2785f*(float)channel0 - 1.9548*(float)channel1)
				/(float)gain/integrationTime;
	}
	else if((ratio < 0.85f) && (ratio >= 0.64f))
	{
		lux_ = (0.5926f*(float)channel0 + 0.1185*(float)channel1)
				/(float)gain/integrationTime;
	}
	else
	{
		lux_ = 0;
	}

	return lux_;
}

/**
  @brief Записывает освещённость в глобальную переменную.
  @param lux_ Освещённость, лк
 */
static void setLux(uint16_t lux_)
{
	osMutexAcquire(mutexLuxHandle, osWaitForever);
	lux = lux_;
	osMutexRelease(mutexLuxHandle);
}

/**
  @brief Возвращает освещённость.
  @return Освещённость, лк
 */
uint16_t getLux()
{
	uint16_t lux_ = 0;
	osMutexAcquire(mutexLuxHandle, osWaitForever);
	lux_ = lux;
	osMutexRelease(mutexLuxHandle);
	return lux_;
}
