#include <stdbool.h>
#include "cmsis_os.h"
#include "i2c.h"


#define STC_REGISTERS_SIZE 13u /**< Длина массива регистров датчика напряжения. */


/**
  Источник тактирования датчика напряжения.
 */
typedef enum
{
	STC_CLOCK_INTERNAL = 0x0, /**< Внутренний источник тактирования датчика напряжения. */
	STC_CLOCK_EXTERNAL = 0x1  /**< Внешний источник тактирования датчика напряжения. */
} StcClock;

/**
  Разрядность АЦП датчика заряда.
 */
typedef enum
{
	STC_RESOLUTION_14_BITS = 0x0, /**< 14-битная разрядность АЦП датчика заряда. */
	STC_RESOLUTION_13_BITS = 0x1, /**< 13-битная разрядность АЦП датчика заряда. */
	STC_RESOLUTION_12_BITS = 0x2  /**< 12-битная разрядность АЦП датчика заряда. */
} StcResolution;

/**
  Калибровка датчика напряжения.
 */
typedef enum
{
	STC_CALIBRATION_OFF = 0x0, /**< Калибровка датчика напряжения не запускается. */
	STC_CALIBRATION_ON	= 0x1  /**< Запуск калибровки датчика напряжения. */
} StcCalibration;

/**
  Режим работы датчика напряжения.
 */
typedef enum
{
	STC_MODE_STANDBY = 0x0, /**< Датчик напряжения не работает. */
	STC_MODE_ACTIVE  = 0x1  /**< Датчик напряжения работает. */
} StcMode;

/**
  Значение вывода IO0 датчика напряжения.
 */
typedef enum
{
	STC_IO_DATA_LOW	 = 0x0, /**< Низкий уровень на выводе IO0 датчика напряжения. */
	STC_IO_DATA_HIGH = 0x1  /**< Высокий уровень на выводе IO0 датчика напряжения. */
} StcIoData;

/**
  Сброс счётчика заряда.
 */
typedef enum
{
	STC_GG_RESET_OFF = 0x0, /**< Сброс счётчика заряда не производится. */
	STC_GG_RESET_ON	 = 0x1  /**< Сброс счётчика заряда. */
} StcGgReset;

/**
  Статус измерения заряда.
 */
typedef enum
{
	STC_GG_CONVERSION_ONGOING  = 0x0, /**< Измерение заряда не закончено. */
	STC_GG_CONVERSION_COMPLETE = 0x1  /**< Измерение заряда закончено. */
} StcGgConversion;

/**
  Статус измерения напряжения.
 */
typedef enum
{
	STC_VTM_CONVERSION_ONGOING  = 0x0, /**< Измерение напряжения не закончено. */
	STC_VTM_CONVERSION_COMPLETE = 0x1  /**< Измерение напряжения закончено. */
} StcVtmConversion;

/**
  Перезагрузка датчика напряжения.
 */
typedef enum
{
	STC_RESET_OFF = 0x0, /**< Перезагрузка датчика напряжения не производится. */
	STC_RESET_ON  = 0x1  /**< Перезагрузка датчика напряжения. */
} StcReset;


volatile uint32_t voltage = 0u;				/**< Напряжение батареи, мВ */
extern osMutexId_t mutexI2cHandle;			/**< Мьютекс для доступа к I2C. */
extern osMutexId_t mutexVoltageHandle;		/**< Мьютекс для доступа к напряжению батареи. */
const uint32_t TASK_VOLTAGE_PERIOD = 4000u; /**< Период запуска задачи чтения напряжения батареи, мс */

uint8_t stcRegisters[STC_REGISTERS_SIZE];		/**< Массив регистров датчика напряжения. */
const uint8_t STC_MODE_INDEX			 = 0u;  /**< Индекс регистра MODE в массиве регистров датчика напряжения. */
const uint8_t STC_CONTROL_INDEX			 = 1u;  /**< Индекс регистра CONTROL в массиве регистров датчика напряжения. */
const uint8_t STC_CHARGE_LOW_INDEX		 = 2u;  /**< Индекс регистра CHARGE_LOW в массиве регистров датчика напряжения. */
const uint8_t STC_CHARGE_HIGH_INDEX		 = 3u;  /**< Индекс регистра CHARGE_HIGH в массиве регистров датчика напряжения. */
const uint8_t STC_COUNTER_LOW_INDEX		 = 4u;  /**< Индекс регистра COUNTER_LOW в массиве регистров датчика напряжения. */
const uint8_t STC_COUNTER_HIGH_INDEX	 = 5u;  /**< Индекс регистра COUNTER_HIGH в массиве регистров датчика напряжения. */
const uint8_t STC_CURRENT_LOW_INDEX		 = 6u;  /**< Индекс регистра CURRENT_LOW в массиве регистров датчика напряжения. */
const uint8_t STC_CURRENT_HIGH_INDEX	 = 7u;  /**< Индекс регистра CURRENT_HIGH в массиве регистров датчика напряжения. */
const uint8_t STC_VOLTAGE_LOW_INDEX		 = 8u;  /**< Индекс регистра VOLTAGE_LOW в массиве регистров датчика напряжения. */
const uint8_t STC_VOLTAGE_HIGH_INDEX	 = 9u;  /**< Индекс регистра VOLTAGE_HIGH в массиве регистров датчика напряжения. */
const uint8_t STC_TEMPERATURE_LOW_INDEX	 = 10u; /**< Индекс регистра TEMPERATURE_LOW в массиве регистров датчика напряжения. */
const uint8_t STC_TEMPERATURE_HIGH_INDEX = 11u; /**< Индекс регистра TEMPERATURE_HIGH в массиве регистров датчика напряжения. */
const uint8_t STC_PART_TYPE_ID_INDEX	 = 12u; /**< Индекс регистра PART_TYPE_ID в массиве регистров датчика напряжения. */

const uint8_t STC_ADDRESS				   = 0x70; /**< Адрес датчика напряжения. */
const uint8_t STC_MODE_ADDRESS			   = 0x00; /**< Адрес регистра MODE датчика напряжения. */
const uint8_t STC_CONTROL_ADDRESS		   = 0x01; /**< Адрес регистра CONTROL датчика напряжения. */
const uint8_t STC_CHARGE_LOW_ADDRESS	   = 0x02; /**< Адрес регистра CHARGE_LOW датчика напряжения. */
const uint8_t STC_CHARGE_HIGH_ADDRESSS	   = 0x03; /**< Адрес регистра CHARGE_HIGH датчика напряжения. */
const uint8_t STC_COUNTER_LOW_ADDRESS	   = 0x04; /**< Адрес регистра COUNTER_LOW датчика напряжения. */
const uint8_t STC_COUNTER_HIGH_ADDRESS	   = 0x05; /**< Адрес регистра COUNTER_HIGH датчика напряжения. */
const uint8_t STC_CURRENT_LOW_ADDRESS	   = 0x06; /**< Адрес регистра CURRENT_LOW датчика напряжения. */
const uint8_t STC_CURRENT_HIGH_ADDRESS	   = 0x07; /**< Адрес регистра CURRENT_HIGH датчика напряжения. */
const uint8_t STC_VOLTAGE_LOW_ADDRESS	   = 0x08; /**< Адрес регистра VOLTAGE_LOW датчика напряжения. */
const uint8_t STC_VOLTAGE_HIGH_ADDRESS	   = 0x09; /**< Адрес регистра VOLTAGE_HIGH датчика напряжения. */
const uint8_t STC_TEMPERATURE_LOW_ADDRESS  = 0x0A; /**< Адрес регистра TEMPERATURE_LOW датчика напряжения. */
const uint8_t STC_TEMPERATURE_HIGH_ADDRESS = 0x0B; /**< Адрес регистра TEMPERATURE_HIGH датчика напряжения. */
const uint8_t STC_PART_TYPE_ID_ADDRESS	   = 0x18; /**< Адрес регистра PART_TYPE_ID датчика напряжения. */

const uint8_t STC_PART_TYPE_ID = 0x10; /**< Серийный номер датчика напряжения. */

const uint8_t STC_CLOCK_OFFSET			= 0u; /**< Сдвиг поля CLOCK в регистре MODE датчика напряжения. */
const uint8_t STC_RESOLUTION_OFFSET		= 1u; /**< Сдвиг поля RESOLUTION в регистре MODE датчика напряжения. */
const uint8_t STC_CALIBRATION_OFFSET	= 3u; /**< Сдвиг поля CALIBRATION в регистре MODE датчика напряжения. */
const uint8_t STC_MODE_OFFSET			= 4u; /**< Сдвиг поля MODE в регистре MODE датчика напряжения. */
const uint8_t STC_IO_DATA_OFFSET		= 0u; /**< Сдвиг поля IO_DATA в регистре CONTROL датчика напряжения. */
const uint8_t STC_GG_RESET_OFFSET		= 1u; /**< Сдвиг поля GG_RESET в регистре CONTROL датчика напряжения. */
const uint8_t STC_GG_CONVERSION_OFFSET	= 2u; /**< Сдвиг поля GG_CONVERSION в регистре CONTROL датчика напряжения. */
const uint8_t STC_VTM_CONVERSION_OFFSET = 3u; /**< Сдвиг поля VTM_CONVERSION в регистре CONTROL датчика напряжения. */
const uint8_t STC_RESET_OFFSET			= 4u; /**< Сдвиг поля RESET в регистре CONTROL датчика напряжения. */

const uint8_t STC_CLOCK_MASK		  = 0x01; /**< Маска для чтения поля CLOCK в регистре MODE датчика напряжения. */
const uint8_t STC_RESOLUTION_MASK	  = 0x06; /**< Маска для чтения поля RESOLUTION в регистре MODE датчика напряжения. */
const uint8_t STC_CALIBRATION_MASK	  = 0x08; /**< Маска для чтения поля CALIBRATION в регистре MODE датчика напряжения. */
const uint8_t STC_MODE_MASK			  = 0x10; /**< Маска для чтения поля MODE в регистре MODE датчика напряжения. */
const uint8_t STC_IO_DATA_MASK		  = 0x01; /**< Маска для чтения поля IO_DATA в регистре CONTROL датчика напряжения. */
const uint8_t STC_GG_RESET_MASK		  = 0x02; /**< Маска для чтения поля GG_RESET в регистре CONTROL датчика напряжения. */
const uint8_t STC_GG_CONVERSION_MASK  = 0x04; /**< Маска для чтения поля GG_CONVERSION в регистре CONTROL датчика напряжения. */
const uint8_t STC_VTM_CONVERSION_MASK = 0x08; /**< Маска для чтения поля VTM_CONVERSION в регистре CONTROL датчика напряжения. */
const uint8_t STC_RESET_MASK		  = 0x10; /**< Маска для чтения поля RESET в регистре CONTROL датчика напряжения. */

const uint32_t STC_TIME_TO_POWER_UP = 100u; /**< Время ожидания до полного запуска датчика напряжения, мс */


static void voltageInit();

static void setDefaultStcRegisterValues();
static void setStcMode(StcClock stcClock,
					   StcResolution stcResolution,
					   StcCalibration stcCalibration,
					   StcMode stcMode);
static void setStcControl(StcIoData stcIoData,
						  StcGgReset stcGgReset,
						  StcGgConversion stcGgConversion,
						  StcVtmConversion stcVtmConversion,
						  StcReset stcReset);
static void setStcChargeLow(uint8_t stcChargeLow);
static void setStcChargeHigh(uint8_t stcChargeHigh);
static void setStcCounterLow(uint8_t stcCounterLow);
static void setStcCounterHigh(uint8_t stcCounterHigh);
static void setStcCurrentLow(uint8_t stcCurrentLow);
static void setStcCurrentHigh(uint8_t stcCurrentHigh);
static void setStcVoltageLow(uint8_t stcVoltageLow);
static void setStcVoltageHigh(uint8_t stcVoltageHigh);
static void setStcTemperatureLow(uint8_t stcTemperatureLow);
static void setStcTemperatureHigh(uint8_t stcTemperatureHigh);
static void setStcPartTypeId();

static StcClock getStcClock();
static StcResolution getStcResolution();
static StcCalibration getStcCalibration();
static StcMode getStcMode();
static StcIoData getStcIoData();
static StcGgReset getStcGgReset();
static StcGgConversion getStcGgConversion();
static StcVtmConversion getStcVtmConversion();
static StcReset getStcReset();
static uint8_t getStcChargeLow();
static uint8_t getStcChargeHigh();
static uint8_t getStcCounterLow();
static uint8_t getStcCounterHigh();
static uint8_t getStcCurrentLow();
static uint8_t getStcCurrentHigh();
static uint8_t getStcVoltageLow();
static uint8_t getStcVoltageHigh();
static uint8_t getStcTemperatureLow();
static uint8_t getStcTemperatureHigh();
static uint8_t getStcPartTypeId();

static bool isNewVoltageDataAvailable();
static void readVoltageData();
static uint32_t calculateVoltage();
static void setVoltage(uint32_t voltage_);


/**
  @brief Задача RTOS чтения напряжения батареи из датчика напряжения.

  Проверяет готовность новых данных для чтения. \n
  В случае готовности читает новые данные в необработанном виде. \n
  Преобразует полученные данные в напряжение батареи в милливольтах. \n
  Записывает напряжение батареи в глобальную переменную. \n
  Период запуска задачи составляет 4 секунды.
 */
void taskVoltageFunction(void *argument)
{
	voltageInit();

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		bool newVoltageDataAvailable = isNewVoltageDataAvailable();
		if(newVoltageDataAvailable == true)
		{
			readVoltageData();
			uint32_t voltage_ = calculateVoltage();
			setVoltage(voltage_);
		}

		osDelayUntil(tick + pdMS_TO_TICKS(TASK_VOLTAGE_PERIOD));
	}
}

/**
  @brief Инициализирует работу с датчиком напряжения.

  Ожидает время до полного запуска датчика напряжения. \n
  Проверяет связь по I2C. \n
  Настраивает и запускает работу датчика напряжения.
 */
static void voltageInit()
{
	osDelay(pdMS_TO_TICKS(STC_TIME_TO_POWER_UP));

	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_IsDeviceReady(&hi2c1, STC_ADDRESS << 1, 1, 1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	setDefaultStcRegisterValues();
	setStcMode(STC_CLOCK_INTERNAL,
			   STC_RESOLUTION_14_BITS,
			   STC_CALIBRATION_OFF,
			   STC_MODE_ACTIVE);
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Write(&hi2c1,
								   STC_ADDRESS << 1,
								   STC_MODE_ADDRESS, I2C_MEMADD_SIZE_8BIT,
								   stcRegisters + STC_MODE_INDEX, 1,
								   1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);
}

/**
  @brief Записывает значения по умолчанию в массив регистров датчика напряжения.
 */
static void setDefaultStcRegisterValues()
{
	setStcMode(STC_CLOCK_INTERNAL,
			   STC_RESOLUTION_14_BITS,
			   STC_CALIBRATION_OFF,
			   STC_MODE_STANDBY);
	setStcControl(STC_IO_DATA_HIGH,
				  STC_GG_RESET_OFF,
				  STC_GG_CONVERSION_COMPLETE,
				  STC_VTM_CONVERSION_COMPLETE,
				  STC_RESET_OFF);
	setStcChargeLow(0);
	setStcChargeHigh(0);
	setStcCounterLow(0);
	setStcCounterHigh(0);
	setStcCurrentLow(0);
	setStcCurrentHigh(0);
	setStcVoltageLow(0);
	setStcVoltageHigh(0);
	setStcTemperatureLow(0);
	setStcTemperatureHigh(0);
	setStcPartTypeId();
}

/**
  @brief Записывает значение регистра MODE в массив регистров датчика напряжения.
  @param stcClock Источник тактирования датчика напряжения.
  @param stcResolution Разрядность АЦП датчика заряда.
  @param stcCalibration Калибровка датчика напряжения.
  @param stcMode Режим работы датчика напряжения.
 */
static void setStcMode(StcClock stcClock,
					   StcResolution stcResolution,
					   StcCalibration stcCalibration,
					   StcMode stcMode)
{
	uint8_t reg = 0;
	reg |= (stcClock << STC_CLOCK_OFFSET);
	reg |= (stcResolution << STC_RESOLUTION_OFFSET);
	reg |= (stcCalibration << STC_CALIBRATION_OFFSET);
	reg |= (stcMode << STC_MODE_OFFSET);
	stcRegisters[STC_MODE_INDEX] = reg;
}

/**
  @brief Записывает значение регистра CONTROL в массив регистров датчика напряжения.
  @param stcIoData Значение вывода IO0 датчика напряжения.
  @param stcGgReset Сброс счётчика заряда.
  @param stcGgConversion Статус измерения заряда.
  @param stcVtmConversion Статус измерения напряжения.
  @param stcReset Перезагрузка датчика напряжения.
 */
static void setStcControl(StcIoData stcIoData,
						  StcGgReset stcGgReset,
						  StcGgConversion stcGgConversion,
						  StcVtmConversion stcVtmConversion,
						  StcReset stcReset)
{
	uint8_t reg = 0;
	reg |= (stcIoData << STC_IO_DATA_OFFSET);
	reg |= (stcGgReset << STC_GG_RESET_OFFSET);
	reg |= (stcGgConversion << STC_GG_CONVERSION_OFFSET);
	reg |= (stcVtmConversion << STC_VTM_CONVERSION_OFFSET);
	reg |= (stcReset << STC_RESET_OFFSET);
	stcRegisters[STC_CONTROL_INDEX] = reg;
}

/**
  @brief Записывает значение регистра CHARGE_LOW в массив регистров датчика напряжения.
  @param stcChargeLow Младший байт заряда.
 */
static void setStcChargeLow(uint8_t stcChargeLow)
{
	stcRegisters[STC_CHARGE_LOW_INDEX] = stcChargeLow;
}

/**
  @brief Записывает значение регистра CHARGE_HIGH в массив регистров датчика напряжения.
  @param stcChargeHigh Старший байт заряда.
 */
static void setStcChargeHigh(uint8_t stcChargeHigh)
{
	stcRegisters[STC_CHARGE_HIGH_INDEX] = stcChargeHigh;
}

/**
  @brief Записывает значение регистра COUNTER_LOW в массив регистров датчика напряжения.
  @param stcCounterLow Младший байт количества измерений.
 */
static void setStcCounterLow(uint8_t stcCounterLow)
{
	stcRegisters[STC_COUNTER_LOW_INDEX] = stcCounterLow;
}

/**
  @brief Записывает значение регистра COUNTER_HIGH в массив регистров датчика напряжения.
  @param stcCounterHigh Старший байт количества измерений.
 */
static void setStcCounterHigh(uint8_t stcCounterHigh)
{
	stcRegisters[STC_COUNTER_HIGH_INDEX] = stcCounterHigh;
}

/**
  @brief Записывает значение регистра CURRENT_LOW в массив регистров датчика напряжения.
  @param stcCurrentLow Младший байт тока.
 */
static void setStcCurrentLow(uint8_t stcCurrentLow)
{
	stcRegisters[STC_CURRENT_LOW_INDEX] = stcCurrentLow;
}

/**
  @brief Записывает значение регистра CURRENT_HIGH в массив регистров датчика напряжения.
  @param stcCurrentHigh Старший байт тока.
 */
static void setStcCurrentHigh(uint8_t stcCurrentHigh)
{
	stcRegisters[STC_CURRENT_HIGH_INDEX] = stcCurrentHigh;
}

/**
  @brief Записывает значение регистра VOLTAGE_LOW в массив регистров датчика напряжения.
  @param stcVoltageLow Младший байт напряжения.
 */
static void setStcVoltageLow(uint8_t stcVoltageLow)
{
	stcRegisters[STC_VOLTAGE_LOW_INDEX] = stcVoltageLow;
}

/**
  @brief Записывает значение регистра VOLTAGE_HIGH в массив регистров датчика напряжения.
  @param stcVoltageHigh Старший байт напряжения.
 */
static void setStcVoltageHigh(uint8_t stcVoltageHigh)
{
	stcRegisters[STC_VOLTAGE_HIGH_INDEX] = stcVoltageHigh;
}

/**
  @brief Записывает значение регистра TEMPERATURE_LOW в массив регистров датчика напряжения.
  @param stcTemperatureLow Младший байт температуры.
 */
static void setStcTemperatureLow(uint8_t stcTemperatureLow)
{
	stcRegisters[STC_TEMPERATURE_LOW_INDEX] = stcTemperatureLow;
}

/**
  @brief Записывает значение регистра TEMPERATURE_HIGH в массив регистров датчика напряжения.
  @param stcTemperatureHigh Старший байт температуры.
 */
static void setStcTemperatureHigh(uint8_t stcTemperatureHigh)
{
	stcRegisters[STC_TEMPERATURE_HIGH_INDEX] = stcTemperatureHigh;
}

/**
  @brief Записывает значение регистра PART_TYPE_ID в массив регистров датчика напряжения.
 */
static void setStcPartTypeId()
{
	stcRegisters[STC_PART_TYPE_ID_INDEX] = STC_PART_TYPE_ID;
}

/**
  @brief Возвращает источник тактирования датчика напряжения из массива регистров датчика напряжения.
  @return Источник тактирования датчика напряжения.
 */
static StcClock getStcClock()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcClock stcClock = (StcClock)
			((reg & STC_CLOCK_MASK) >> STC_CLOCK_OFFSET);
	return stcClock;
}

/**
  @brief Возвращает разрядность АЦП датчика заряда из массива регистров датчика напряжения.
  @return Разрядность АЦП датчика заряда.
 */
static StcResolution getStcResolution()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcResolution stcResolution = (StcResolution)
			((reg & STC_RESOLUTION_MASK) >> STC_RESOLUTION_OFFSET);
	return stcResolution;
}

/**
  @brief Возвращает калибровку датчика напряжения из массива регистров датчика напряжения.
  @return Калибровка датчика напряжения.
 */
static StcCalibration getStcCalibration()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcCalibration stcCalibration = (StcCalibration)
			((reg & STC_CALIBRATION_MASK) >> STC_CALIBRATION_OFFSET);
	return stcCalibration;
}

/**
  @brief Возвращает режим работы датчика напряжения из массива регистров датчика напряжения.
  @return Режим работы датчика напряжения.
 */
static StcMode getStcMode()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcMode stcMode = (StcMode)
			((reg & STC_MODE_MASK) >> STC_MODE_OFFSET);
	return stcMode;
}

/**
  @brief Возвращает значение вывода IO0 датчика напряжения из массива регистров датчика напряжения.
  @return Значение вывода IO0 датчика напряжения.
 */
static StcIoData getStcIoData()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcIoData stcIoData = (StcIoData)
			((reg & STC_IO_DATA_MASK) >> STC_IO_DATA_OFFSET);
	return stcIoData;
}

/**
  @brief Возвращает сброс счётчика заряда из массива регистров датчика напряжения.
  @return Сброс счётчика заряда.
 */
static StcGgReset getStcGgReset()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcGgReset stcGgReset = (StcGgReset)
			((reg & STC_GG_RESET_MASK) >> STC_GG_RESET_OFFSET);
	return stcGgReset;
}

/**
  @brief Возвращает статус измерения заряда из массива регистров датчика напряжения.
  @return Статус измерения заряда.
 */
static StcGgConversion getStcGgConversion()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcGgConversion stcGgConversion = (StcGgConversion)
			((reg & STC_GG_CONVERSION_MASK) >> STC_GG_CONVERSION_OFFSET);
	return stcGgConversion;
}

/**
  @brief Возвращает статус измерения напряжения из массива регистров датчика напряжения.
  @return Статус измерения напряжения.
 */
static StcVtmConversion getStcVtmConversion()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcVtmConversion stcVtmConversion = (StcVtmConversion)
			((reg & STC_VTM_CONVERSION_MASK) >> STC_VTM_CONVERSION_OFFSET);
	return stcVtmConversion;
}

/**
  @brief Возвращает перезагрузку датчика напряжения из массива регистров датчика напряжения.
  @return Перезагрузка датчика напряжения.
 */
static StcReset getStcReset()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcReset stcReset = (StcReset)
			((reg & STC_RESET_MASK) >> STC_RESET_OFFSET);
	return stcReset;
}

/**
  @brief Возвращает младший байт заряда из массива регистров датчика напряжения.
  @return Младший байт заряда.
 */
static uint8_t getStcChargeLow()
{
	return stcRegisters[STC_CHARGE_LOW_INDEX];
}

/**
  @brief Возвращает старший байт заряда из массива регистров датчика напряжения.
  @return Старший байт заряда.
 */
static uint8_t getStcChargeHigh()
{
	return stcRegisters[STC_CHARGE_HIGH_INDEX];
}

/**
  @brief Возвращает младший байт количества измерений из массива регистров датчика напряжения.
  @return Младший байт количества измерений.
 */
static uint8_t getStcCounterLow()
{
	return stcRegisters[STC_COUNTER_LOW_INDEX];
}

/**
  @brief Возвращает старший байт количества измерений из массива регистров датчика напряжения.
  @return Старший байт количества измерений.
 */
static uint8_t getStcCounterHigh()
{
	return stcRegisters[STC_COUNTER_HIGH_INDEX];
}

/**
  @brief Возвращает младший байт тока из массива регистров датчика напряжения.
  @return Младший байт тока.
 */
static uint8_t getStcCurrentLow()
{
	return stcRegisters[STC_CURRENT_LOW_INDEX];
}

/**
  @brief Возвращает старший байт тока из массива регистров датчика напряжения.
  @return Старший байт тока.
 */
static uint8_t getStcCurrentHigh()
{
	return stcRegisters[STC_CURRENT_HIGH_INDEX];
}

/**
  @brief Возвращает младший байт напряжения из массива регистров датчика напряжения.
  @return Младший байт напряжения.
 */
static uint8_t getStcVoltageLow()
{
	return stcRegisters[STC_VOLTAGE_LOW_INDEX];
}

/**
  @brief Возвращает страший байт напряжения из массива регистров датчика напряжения.
  @return Старший байт напряжения.
 */
static uint8_t getStcVoltageHigh()
{
	return stcRegisters[STC_VOLTAGE_HIGH_INDEX];
}

/**
  @brief Возвращает младший байт температуры из массива регистров датчика напряжения.
  @return Младший байт температуры.
 */
static uint8_t getStcTemperatureLow()
{
	return stcRegisters[STC_TEMPERATURE_LOW_INDEX];
}

/**
  @brief Возвращает старший байт температуры из массива регистров датчика напряжения.
  @return Старший байт температуры.
 */
static uint8_t getStcTemperatureHigh()
{
	return stcRegisters[STC_TEMPERATURE_HIGH_INDEX];
}

/**
  @brief Возвращает серийный номер датчика напряжения из массива регистров датчика напряжения.
  @return Серийный номер датчика напряжения.
 */
static uint8_t getStcPartTypeId()
{
	return stcRegisters[STC_PART_TYPE_ID_INDEX];
}

/**
  @brief Проверяет готовность новых данных датчика напряжения для чтения.
  @return Готовность новых данных датчика напряжения для чтения. \n
  true - Новые данные датчика напряжения готовы для чтения. \n
  false - Новые данные датчика напряжения не готовы для чтения.
 */
static bool isNewVoltageDataAvailable()
{
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c1,
								  STC_ADDRESS << 1,
								  STC_CONTROL_ADDRESS, I2C_MEMADD_SIZE_8BIT,
								  stcRegisters + STC_CONTROL_INDEX, 1,
								  1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	StcVtmConversion stcVtmConversion = getStcVtmConversion();
	if(stcVtmConversion == STC_VTM_CONVERSION_COMPLETE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
  @brief Читает новые данные датчика напряжения в необработанном виде.
 */
static void readVoltageData()
{
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	HAL_StatusTypeDef status = HAL_ERROR;
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c1,
								  STC_ADDRESS << 1,
								  STC_VOLTAGE_LOW_ADDRESS, I2C_MEMADD_SIZE_8BIT,
								  stcRegisters + STC_VOLTAGE_LOW_INDEX, 2,
								  1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);
}

/**
  @brief Преобразует полученные данные из датчика напряжения в напряжение батареи в милливольтах.
  @return Напряжение батареи, мВ
 */
static uint32_t calculateVoltage()
{
	uint32_t voltage_ = 0;

	uint8_t voltageHigh = getStcVoltageHigh();
	uint8_t voltageLow = getStcVoltageLow();
	uint16_t voltageData = 0;
	voltageData = ((uint16_t)voltageHigh << 8) | (uint16_t)voltageLow;
	voltage_ = (float)voltageData*2.44f;

	return voltage_;
}

/**
  @brief Записывает напряжение батареи в глобальную переменную.
  @param voltage_ Напряжение батареи, мВ
 */
static void setVoltage(uint32_t voltage_)
{
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage = voltage_;
	osMutexRelease(mutexVoltageHandle);
}

/**
  @brief Возвращает напряжение батареи.
  @return Напряжение батареи, мВ
 */
uint32_t getVoltage()
{
	uint32_t voltage_ = 0;
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage_ = voltage;
	osMutexRelease(mutexVoltageHandle);
	return voltage_;
}
