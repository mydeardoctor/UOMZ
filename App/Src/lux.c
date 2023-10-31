#include <stdbool.h>
#include "cmsis_os.h"
#include "i2c.h"


#define LTR_REGISTERS_SIZE 9u


typedef enum
{
	LTR_MODE_STANDBY = 0x0,
	LTR_MODE_ACTIVE	 = 0x1
} LtrMode;

typedef enum
{
	LTR_RESET_OFF = 0x0,
	LTR_RESET_ON  = 0x1
} LtrReset;

typedef enum
{
	LTR_GAIN_1X  = 0x0,
	LTR_GAIN_2X  = 0x1,
	LTR_GAIN_4X  = 0x2,
	LTR_GAIN_8X  = 0x3,
	LTR_GAIN_48X = 0x6,
	LTR_GAIN_96X = 0x7
} LtrGain;

typedef enum
{
	LTR_MEASUREMENT_RATE_50MS	= 0x0,
	LTR_MEASUREMENT_RATE_100MS	= 0x1,
	LTR_MEASUREMENT_RATE_200MS	= 0x2,
	LTR_MEASUREMENT_RATE_500MS	= 0x3,
	LTR_MEASUREMENT_RATE_1000MS = 0x4,
	LTR_MEASUREMENT_RATE_2000MS = 0x5
} LtrMeasurementRate;

typedef enum
{
	LTR_INTEGRATION_TIME_100MS	= 0x0,
	LTR_INTEGRATION_TIME_50MS	= 0x1,
	LTR_INTEGRATION_TIME_200MS	= 0x2,
	LTR_INTEGRATION_TIME_400MS	= 0x3,
	LTR_INTEGRATIONG_TIME_150MS = 0x4,
	LTR_INTEGRATION_TIME_250MS	= 0x5,
	LTR_INTEGRATION_TIME_300MS	= 0x6,
	LTR_INTEGRATION_TIME_350MS	= 0x7
} LtrIntegrationTime;

typedef enum
{
	LTR_DATA_STATUS_OLD = 0x0,
	LTR_DATA_STATUS_NEW = 0x1
} LtrDataStatus;

typedef enum
{
	LTR_DATA_GAIN_RANGE_1X	= 0x0,
	LTR_DATA_GAIN_RANGE_2X	= 0x1,
	LTR_DATA_GAIN_RANGE_4X	= 0x2,
	LTR_DATA_GAIN_RANGE_8X	= 0x3,
	LTR_DATA_GAIN_RANGE_48X = 0x6,
	LTR_DATA_GAIN_RANGE_96X = 0x7
} LtrDataGainRange;

typedef enum
{
	LTR_DATA_VALIDITY_VALID	  = 0x0,
	LTR_DATA_VALIDITY_INVALID = 0x1
} LtrDataValidity;


volatile uint16_t lux = 0u;
extern osMutexId_t mutexI2cHandle;
extern osMutexId_t mutexLuxHandle;
const uint32_t TASK_LUX_PERIOD = 100u;

uint8_t ltrRegisters[LTR_REGISTERS_SIZE];
const uint8_t LTR_CONTROL_INDEX			 = 0u;
const uint8_t LTR_MEASUREMENT_RATE_INDEX = 1u;
const uint8_t LTR_PART_ID_INDEX			 = 2u;
const uint8_t LTR_MANUFACTURER_ID_INDEX	 = 3u;
const uint8_t LTR_CHANNEL1_LOW_INDEX	 = 4u;
const uint8_t LTR_CHANNEL1_HIGH_INDEX	 = 5u;
const uint8_t LTR_CHANNEL0_LOW_INDEX	 = 6u;
const uint8_t LTR_CHANNEL0_HIGH_INDEX	 = 7u;
const uint8_t LTR_STATUS_INDEX	   	 	 = 8u;

const uint8_t LTR_ADDRESS		 		   = 0x29;
const uint8_t LTR_CONTROL_ADDRESS		   = 0x80;
const uint8_t LTR_MEASUREMENT_RATE_ADDRESS = 0x85;
const uint8_t LTR_PART_ID_ADDRESS	 	   = 0x86;
const uint8_t LTR_MANUFACTURER_ID_ADDRESS  = 0x87;
const uint8_t LTR_CHANNEL1_LOW_ADDRESS	   = 0x88;
const uint8_t LTR_CHANNEL1_HIGH_ADDRESS	   = 0x89;
const uint8_t LTR_CHANNEL0_LOW_ADDRESS	   = 0x8A;
const uint8_t LTR_CHANNEL0_HIGH_ADDRESS	   = 0x8B;
const uint8_t LTR_STATUS_ADDRESS		   = 0x8C;

const uint8_t LTR_PART_NUMBER_ID  = 0x0A;
const uint8_t LTR_REVISION_ID	  = 0x00;
const uint8_t LTR_MANUFACTURER_ID = 0x05;

const uint8_t LTR_MODE_OFFSET			  = 0u;
const uint8_t LTR_RESET_OFFSET		  	  = 1u;
const uint8_t LTR_GAIN_OFFSET			  = 2u;
const uint8_t LTR_MEASUREMENT_RATE_OFFSET = 0u;
const uint8_t LTR_INTEGRATION_TIME_OFFSET = 3u;
const uint8_t LTR_REVISION_ID_OFFSET	  = 0u;
const uint8_t LTR_PART_NUMBER_ID_OFFSET	  = 4u;
const uint8_t LTR_DATA_STATUS_OFFSET	  = 2u;
const uint8_t LTR_DATA_GAIN_RANGE_OFFSET  = 4u;
const uint8_t LTR_DATA_VALIDITY_OFFSET	  = 7u;

const uint8_t LTR_MODE_MASK				  = 0x01;
const uint8_t LTR_RESET_MASK			  = 0x02;
const uint8_t LTR_GAIN_MASK				  = 0x1C;
const uint8_t LTR_MEASUREMENT_RATE_MASK	  = 0x07;
const uint8_t LTR_INTEGRATION_TIME_MASK	  = 0x38;
const uint8_t LTR_REVISION_ID_MASK		  = 0x0F;
const uint8_t LTR_PART_NUMBER_ID_MASK	  = 0xF0;
const uint8_t LTR_DATA_STATUS_MASK		  = 0x04;
const uint8_t LTR_DATA_GAIN_RANGE_MASK	  = 0x70;
const uint8_t LTR_DATA_VALIDITY_MASK	  = 0x80;

const uint32_t LTR_TIME_TO_POWER_UP = 100u;
const uint32_t LTR_TIME_TO_WAKE_UP	= 10u;


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

static void setLtrMeasurementRate(LtrMeasurementRate ltrMeasurementRate,
								  LtrIntegrationTime ltrIntegrationTime)
{
	uint8_t reg = 0;
	reg |= (ltrMeasurementRate << LTR_MEASUREMENT_RATE_OFFSET);
	reg |= (ltrIntegrationTime << LTR_INTEGRATION_TIME_OFFSET);
	ltrRegisters[LTR_MEASUREMENT_RATE_INDEX] = reg;
}

static void setLtrPartId()
{
	uint8_t reg = 0;
	reg |= (LTR_REVISION_ID << LTR_REVISION_ID_OFFSET);
	reg |= (LTR_PART_NUMBER_ID << LTR_PART_NUMBER_ID_OFFSET);
	ltrRegisters[LTR_PART_ID_INDEX] = reg;
}

static void setLtrManufacturerId()
{
	ltrRegisters[LTR_MANUFACTURER_ID_INDEX] = LTR_MANUFACTURER_ID;
}

static void setLtrChannel1Low(uint8_t channel1Low)
{
	ltrRegisters[LTR_CHANNEL1_LOW_INDEX] = channel1Low;
}

static void setLtrChannel1High(uint8_t channel1High)
{
	ltrRegisters[LTR_CHANNEL1_HIGH_INDEX] = channel1High;
}

static void setLtrChannel0Low(uint8_t channel0Low)
{
	ltrRegisters[LTR_CHANNEL0_LOW_INDEX] = channel0Low;
}

static void setLtrChannel0High(uint8_t channel0High)
{
	ltrRegisters[LTR_CHANNEL0_HIGH_INDEX] = channel0High;
}

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

static LtrMode getLtrMode()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrMode ltrMode = (LtrMode)((reg & LTR_MODE_MASK) >> LTR_MODE_OFFSET);
	return ltrMode;
}

static LtrReset getLtrReset()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrReset ltrReset = (LtrReset)((reg & LTR_RESET_MASK) >> LTR_RESET_OFFSET);
	return ltrReset;
}

static LtrGain getLtrGain()
{
	uint8_t reg = ltrRegisters[LTR_CONTROL_INDEX];
	LtrGain ltrGain = (LtrGain)((reg & LTR_GAIN_MASK) >> LTR_GAIN_OFFSET);
	return ltrGain;
}

static LtrMeasurementRate getLtrMeasurementRate()
{
	uint8_t reg = ltrRegisters[LTR_MEASUREMENT_RATE_INDEX];
	LtrMeasurementRate ltrMeasurementRate = (LtrMeasurementRate)
			((reg & LTR_MEASUREMENT_RATE_MASK) >> LTR_MEASUREMENT_RATE_OFFSET);
	return ltrMeasurementRate;
}

static LtrIntegrationTime getLtrIntegrationTime()
{
	uint8_t reg = ltrRegisters[LTR_MEASUREMENT_RATE_INDEX];
	LtrIntegrationTime ltrIntegrationTime = (LtrIntegrationTime)
			((reg & LTR_INTEGRATION_TIME_MASK) >> LTR_INTEGRATION_TIME_OFFSET);
	return ltrIntegrationTime;
}

static uint8_t getLtrRevisionId()
{
	uint8_t reg = ltrRegisters[LTR_PART_ID_INDEX];
	uint8_t ltrRevisionId =
			(reg & LTR_REVISION_ID_MASK) >> LTR_REVISION_ID_OFFSET;
	return ltrRevisionId;
}

static uint8_t getLtrPartNumberId()
{
	uint8_t reg = ltrRegisters[LTR_PART_ID_INDEX];
	uint8_t ltrPartNumberId =
			(reg & LTR_PART_NUMBER_ID_MASK) >> LTR_PART_NUMBER_ID_OFFSET;
	return ltrPartNumberId;
}

static uint8_t getLtrManufacturerId()
{
	return ltrRegisters[LTR_MANUFACTURER_ID_INDEX];
}

static uint8_t getLtrChannel1Low()
{
	return ltrRegisters[LTR_CHANNEL1_LOW_INDEX];
}

static uint8_t getLtrChannel1High()
{
	return ltrRegisters[LTR_CHANNEL1_HIGH_INDEX];
}

static uint8_t getLtrChannel0Low()
{
	return ltrRegisters[LTR_CHANNEL0_LOW_INDEX];
}

static uint8_t getLtrChannel0High()
{
	return ltrRegisters[LTR_CHANNEL0_HIGH_INDEX];
}

static LtrDataStatus getLtrDataStatus()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataStatus ltrDataStatus = (LtrDataStatus)
			((reg & LTR_DATA_STATUS_MASK) >> LTR_DATA_STATUS_OFFSET);
	return ltrDataStatus;
}

static LtrDataGainRange getLtrDataGainRange()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataGainRange ltrDataGainRange = (LtrDataGainRange)
			((reg & LTR_DATA_GAIN_RANGE_MASK) >> LTR_DATA_GAIN_RANGE_OFFSET);
	return ltrDataGainRange;
}

static LtrDataValidity getLtrDataValidity()
{
	uint8_t reg = ltrRegisters[LTR_STATUS_INDEX];
	LtrDataValidity ltrDataValidity = (LtrDataValidity)
			((reg & LTR_DATA_VALIDITY_MASK) >> LTR_DATA_VALIDITY_OFFSET);
	return ltrDataValidity;
}

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

static uint16_t calculateLux()
{
	static uint16_t lux_ = 0;

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

static void setLux(uint16_t lux_)
{
	osMutexAcquire(mutexLuxHandle, osWaitForever);
	lux = lux_;
	osMutexRelease(mutexLuxHandle);
}

uint16_t getLux()
{
	uint16_t lux_ = 0;
	osMutexAcquire(mutexLuxHandle, osWaitForever);
	lux_ = lux;
	osMutexRelease(mutexLuxHandle);
	return lux_;
}
