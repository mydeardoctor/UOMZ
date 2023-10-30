#include <stdbool.h>
#include "cmsis_os.h"
#include "i2c.h"


#define STC_REGISTERS_SIZE 13u


typedef enum
{
	STC_CLOCK_INTERNAL = 0x0,
	STC_CLOCK_EXTERNAL = 0x1
} StcClock;

typedef enum
{
	STC_RESOLUTION_14_BITS = 0x0,
	STC_RESOLUTION_13_BITS = 0x1,
	STC_RESOLUTION_12_BITS = 0x2
} StcResolution;

typedef enum
{
	STC_CALIBRATION_OFF = 0x0,
	STC_CALIBRATION_ON	= 0x1
} StcCalibration;

typedef enum
{
	STC_MODE_STANDBY = 0x0,
	STC_MODE_ACTIVE  = 0x1
} StcMode;

typedef enum
{
	STC_IO_DATA_LOW	 = 0x0,
	STC_IO_DATA_HIGH = 0x1
} StcIoData;

typedef enum
{
	STC_GG_RESET_OFF = 0x0,
	STC_GG_RESET_ON	 = 0x1
} StcGgReset;

typedef enum
{
	STC_GG_CONVERSION_ONGOING  = 0x0,
	STC_GG_CONVERSION_COMPLETE = 0x1
} StcGgConversion;

typedef enum
{
	STC_VTM_CONVERSION_ONGOING  = 0x0,
	STC_VTM_CONVERSION_COMPLETE = 0x1
} StcVtmConversion;

typedef enum
{
	STC_RESET_OFF = 0x0,
	STC_RESET_ON  = 0x1
} StcReset;


volatile uint32_t voltage = 0u;
extern osMutexId_t mutexI2cHandle;
extern osMutexId_t mutexVoltageHandle;
const uint32_t TASK_VOLTAGE_PERIOD = 4000u;

uint8_t stcRegisters[STC_REGISTERS_SIZE];
const uint8_t STC_MODE_INDEX			 = 0u;
const uint8_t STC_CONTROL_INDEX			 = 1u;
const uint8_t STC_CHARGE_LOW_INDEX		 = 2u;
const uint8_t STC_CHARGE_HIGH_INDEX		 = 3u;
const uint8_t STC_COUNTER_LOW_INDEX		 = 4u;
const uint8_t STC_COUNTER_HIGH_INDEX	 = 5u;
const uint8_t STC_CURRENT_LOW_INDEX		 = 6u;
const uint8_t STC_CURRENT_HIGH_INDEX	 = 7u;
const uint8_t STC_VOLTAGE_LOW_INDEX		 = 8u;
const uint8_t STC_VOLTAGE_HIGH_INDEX	 = 9u;
const uint8_t STC_TEMPERATURE_LOW_INDEX	 = 10u;
const uint8_t STC_TEMPERATURE_HIGH_INDEX = 11u;
const uint8_t STC_PART_TYPE_ID_INDEX	 = 12u;

const uint8_t STC_ADDRESS = 0x70;
const uint8_t STC_MODE_ADDRESS			   = 0x00;
const uint8_t STC_CONTROL_ADDRESS		   = 0x01;
const uint8_t STC_CHARGE_LOW_ADDRESS	   = 0x02;
const uint8_t STC_CHARGE_HIGH_ADDRESSS	   = 0x03;
const uint8_t STC_COUNTER_LOW_ADDRESS	   = 0x04;
const uint8_t STC_COUNTER_HIGH_ADDRESS	   = 0x05;
const uint8_t STC_CURRENT_LOW_ADDRESS	   = 0x06;
const uint8_t STC_CURRENT_HIGH_ADDRESS	   = 0x07;
const uint8_t STC_VOLTAGE_LOW_ADDRESS	   = 0x08;
const uint8_t STC_VOLTAGE_HIGH_ADDRESS	   = 0x09;
const uint8_t STC_TEMPERATURE_LOW_ADDRESS  = 0x0A;
const uint8_t STC_TEMPERATURE_HIGH_ADDRESS = 0x0B;
const uint8_t STC_PART_TYPE_ID_ADDRESS	   = 0x18;

const uint8_t PART_TYPE_ID = 0x10;

const uint8_t STC_CLOCK_OFFSET			= 0u;
const uint8_t STC_RESOLUTION_OFFSET		= 1u;
const uint8_t STC_CALIBRATION_OFFSET	= 3u;
const uint8_t STC_MODE_OFFSET			= 4u;
const uint8_t STC_IO_DATA_OFFSET		= 0u;
const uint8_t STC_GG_RESET_OFFSET		= 1u;
const uint8_t STC_GG_CONVERSION_OFFSET	= 2u;
const uint8_t STC_VTM_CONVERSION_OFFSET = 3u;
const uint8_t STC_RESET_OFFSET			= 4u;

const uint8_t STC_CLOCK_MASK		  = 0x01;
const uint8_t STC_RESOLUTION_MASK	  = 0x06;
const uint8_t STC_CALIBRATION_MASK	  = 0x08;
const uint8_t STC_MODE_MASK			  = 0x10;
const uint8_t STC_IO_DATA_MASK		  = 0x01;
const uint8_t STC_GG_RESET_MASK		  = 0x02;
const uint8_t STC_GG_CONVERSION_MASK  = 0x04;
const uint8_t STC_VTM_CONVERSION_MASK = 0x08;
const uint8_t STC_RESET_MASK		  = 0x10;


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
static void setStcPartTypeId(uint8_t stcPartTypeId);

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

static void readVoltageData();
static uint32_t calculateVoltage();
static void setVoltage(uint32_t voltage_);


void taskVoltageFunction(void *argument)
{
	voltageInit();

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		readVoltageData();
		uint32_t voltage_ = calculateVoltage();
		setVoltage(voltage_);

		osDelayUntil(tick + pdMS_TO_TICKS(TASK_VOLTAGE_PERIOD));
	}
}

static void voltageInit()
{
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
	setStcPartTypeId(PART_TYPE_ID);
}

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

static void setStcChargeLow(uint8_t stcChargeLow)
{
	stcRegisters[STC_CHARGE_LOW_INDEX] = stcChargeLow;
}

static void setStcChargeHigh(uint8_t stcChargeHigh)
{
	stcRegisters[STC_CHARGE_HIGH_INDEX] = stcChargeHigh;
}

static void setStcCounterLow(uint8_t stcCounterLow)
{
	stcRegisters[STC_COUNTER_LOW_INDEX] = stcCounterLow;
}

static void setStcCounterHigh(uint8_t stcCounterHigh)
{
	stcRegisters[STC_COUNTER_HIGH_INDEX] = stcCounterHigh;
}

static void setStcCurrentLow(uint8_t stcCurrentLow)
{
	stcRegisters[STC_CURRENT_LOW_INDEX] = stcCurrentLow;
}

static void setStcCurrentHigh(uint8_t stcCurrentHigh)
{
	stcRegisters[STC_CURRENT_HIGH_INDEX] = stcCurrentHigh;
}

static void setStcVoltageLow(uint8_t stcVoltageLow)
{
	stcRegisters[STC_VOLTAGE_LOW_INDEX] = stcVoltageLow;
}

static void setStcVoltageHigh(uint8_t stcVoltageHigh)
{
	stcRegisters[STC_VOLTAGE_HIGH_INDEX] = stcVoltageHigh;
}

static void setStcTemperatureLow(uint8_t stcTemperatureLow)
{
	stcRegisters[STC_TEMPERATURE_LOW_INDEX] = stcTemperatureLow;
}

static void setStcTemperatureHigh(uint8_t stcTemperatureHigh)
{
	stcRegisters[STC_TEMPERATURE_HIGH_INDEX] = stcTemperatureHigh;
}

static void setStcPartTypeId(uint8_t stcPartTypeId)
{
	stcRegisters[STC_PART_TYPE_ID_INDEX] = stcPartTypeId;
}

static StcClock getStcClock()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcClock stcClock = (StcClock)
			((reg & STC_CLOCK_MASK) >> STC_CLOCK_OFFSET);
	return stcClock;
}

static StcResolution getStcResolution()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcResolution stcResolution = (StcResolution)
			((reg & STC_RESOLUTION_MASK) >> STC_RESOLUTION_OFFSET);
	return stcResolution;
}

static StcCalibration getStcCalibration()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcCalibration stcCalibration = (StcCalibration)
			((reg & STC_CALIBRATION_MASK) >> STC_CALIBRATION_OFFSET);
	return stcCalibration;
}

static StcMode getStcMode()
{
	uint8_t reg = stcRegisters[STC_MODE_INDEX];
	StcMode stcMode = (StcMode)
			((reg & STC_MODE_MASK) >> STC_MODE_OFFSET);
	return stcMode;
}

static StcIoData getStcIoData()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcIoData stcIoData = (StcIoData)
			((reg & STC_IO_DATA_MASK) >> STC_IO_DATA_OFFSET);
	return stcIoData;
}

static StcGgReset getStcGgReset()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcGgReset stcGgReset = (StcGgReset)
			((reg & STC_GG_RESET_MASK) >> STC_GG_RESET_OFFSET);
	return stcGgReset;
}

static StcGgConversion getStcGgConversion()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcGgConversion stcGgConversion = (StcGgConversion)
			((reg & STC_GG_CONVERSION_MASK) >> STC_GG_CONVERSION_OFFSET);
	return stcGgConversion;
}

static StcVtmConversion getStcVtmConversion()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcVtmConversion stcVtmConversion = (StcVtmConversion)
			((reg & STC_VTM_CONVERSION_MASK) >> STC_VTM_CONVERSION_OFFSET);
	return stcVtmConversion;
}

static StcReset getStcReset()
{
	uint8_t reg = stcRegisters[STC_CONTROL_INDEX];
	StcReset stcReset = (StcReset)
			((reg & STC_RESET_MASK) >> STC_RESET_OFFSET);
	return stcReset;
}

static uint8_t getStcChargeLow()
{
	return stcRegisters[STC_CHARGE_LOW_INDEX];
}

static uint8_t getStcChargeHigh()
{
	return stcRegisters[STC_CHARGE_HIGH_INDEX];
}

static uint8_t getStcCounterLow()
{
	return stcRegisters[STC_COUNTER_LOW_INDEX];
}

static uint8_t getStcCounterHigh()
{
	return stcRegisters[STC_COUNTER_HIGH_INDEX];
}

static uint8_t getStcCurrentLow()
{
	return stcRegisters[STC_CURRENT_LOW_INDEX];
}

static uint8_t getStcCurrentHigh()
{
	return stcRegisters[STC_CURRENT_HIGH_INDEX];
}

static uint8_t getStcVoltageLow()
{
	return stcRegisters[STC_VOLTAGE_LOW_INDEX];
}

static uint8_t getStcVoltageHigh()
{
	return stcRegisters[STC_VOLTAGE_HIGH_INDEX];
}

static uint8_t getStcTemperatureLow()
{
	return stcRegisters[STC_TEMPERATURE_LOW_INDEX];
}

static uint8_t getStcTemperatureHigh()
{
	return stcRegisters[STC_TEMPERATURE_HIGH_INDEX];
}

static uint8_t getStcPartTypeId()
{
	return stcRegisters[STC_PART_TYPE_ID_INDEX];
}

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

static uint32_t calculateVoltage()
{
	static uint32_t voltage_ = 0;

	uint8_t voltageHigh = getStcVoltageHigh();
	uint8_t voltageLow = getStcVoltageLow();
	uint16_t voltageData = 0;
	voltageData = ((uint16_t)voltageHigh << 8) | (uint16_t)voltageLow;
	voltage_ = (float)voltageData*2.44f;

	return voltage_;
}

static void setVoltage(uint32_t voltage_)
{
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage = voltage_;
	osMutexRelease(mutexVoltageHandle);
}

uint32_t getVoltage()
{
	uint32_t voltage_ = 0;
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage_ = voltage;
	osMutexRelease(mutexVoltageHandle);
	return voltage_;
}
