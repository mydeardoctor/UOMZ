#include <stdbool.h>
#include "cmsis_os.h"
#include "i2c.h"

//TODO убрать лишние геттеры и сеттеры

#define STC_REGISTERS_SIZE 20u


typedef enum
{
	AUTO_DETECT = 0x0,
	EXTERNAL_CLOCK = 0x1
} ExternalClockOption;

typedef enum
{
	RESOLUTION_14_BITS = 0x0,
	RESOLUTION_13_BITS = 0x1,
	RESOLUTION_12_BITS = 0x2
} ResolutionOption;

typedef enum
{
	NO_CALIBRATION = 0x0,
	CALIBRATION = 0x1
} CalibrationOption;

typedef enum
{
	STANDBY_MODE   = 0x0,
	OPERATING_MODE = 0x1
} RunOption;

typedef enum
{
	IO0_LOW = 0x0,
	IO0_HIGH = 0x1
} IO0DataOption;

typedef enum
{
	GG_NO_RESET = 0x0,
	GG_RESET = 0x1
} GGResetOption;

typedef enum
{
	GG_CONVERSION_ONGOING = 0x0,
	GG_END_OF_CONVERSION = 0x1
} GgEocOption;

typedef enum
{
	VTM_CONVERSION_ONGOING = 0x0,
	VTM_END_OF_CONVERSION = 0x1
} VtmEocOption;

typedef enum
{
	POR_NO_RESET = 0x0,
	POR_RESET	= 0x1
} PorResetOption;


volatile uint32_t voltage = 0u;
extern osMutexId_t mutexI2cHandle;
extern osMutexId_t mutexVoltageHandle;

uint8_t stcRegisters[STC_REGISTERS_SIZE];
const uint8_t MODE_INDEX = 0u;
const uint8_t CTRL_INDEX = 1u;
const uint8_t CHARGE_LOW_INDEX = 2u;
const uint8_t CHARGE_HIGH_INDEX = 3u;
const uint8_t COUNTER_LOW_INDEX = 4u;
const uint8_t COUNTER_HIGH_INDEX = 5u;
const uint8_t CURRENT_LOW_INDEX = 6u;
const uint8_t CURRENT_HIGH_INDEX = 7u;
const uint8_t VOLTAGE_LOW_INDEX = 8u;
const uint8_t VOLTAGE_HIGH_INDEX = 9u;
const uint8_t TEMPERATURE_LOW_INDEX = 10u;
const uint8_t TEMPERATURE_HIGH_INDEX = 11;
const uint8_t ID0_INDEX = 12u;
const uint8_t ID1_INDEX = 13u;
const uint8_t ID2_INDEX = 14u;
const uint8_t ID3_INDEX = 15u;
const uint8_t ID4_INDEX = 16u;
const uint8_t ID5_INDEX = 17u;
const uint8_t ID6_INDEX = 18u;
const uint8_t ID7_INDEX = 19u;

const uint8_t STC_ADDRESS = 0x70;
const uint8_t MODE_ADDRESS = 0x00;
const uint8_t CTRL_ADDRESS = 0x01;
const uint8_t CHARGE_LOW_ADDRESS = 0x02;
const uint8_t CHARGE_HIGH_ADDRESSS = 0x03;
const uint8_t COUNTER_LOW_ADDRESS = 0x04;
const uint8_t COUNTER_HIGH_ADDRESS = 0x05;
const uint8_t CURRENT_LOW_ADDRESS = 0x06;
const uint8_t CURRENT_HIGH_ADDRESS = 0x07;
const uint8_t VOLTAGE_LOW_ADDRESS = 0x08;
const uint8_t VOLTAGE_HIGH_ADDRESS = 0x09;
const uint8_t TEMPERATURE_LOW_ADDRESS = 0xA;
const uint8_t TEMPERATURE_HIGH_ADDRESS = 0xB;
const uint8_t ID0_ADDRESS = 0x18;
const uint8_t ID1_ADDRESS = 0x19;
const uint8_t ID2_ADDRESS = 0x1A;
const uint8_t ID3_ADDRESS = 0x1B;
const uint8_t ID4_ADDRESS = 0x1C;
const uint8_t ID5_ADDRESS = 0x1D;
const uint8_t ID6_ADDRESS = 0x1E;
const uint8_t ID7_ADDRESS = 0x1F;

const uint8_t PART_TYPE_ID = 0x10;

const uint8_t EXTERNAL_CLOCK_OFFSET = 0u;
const uint8_t RESOLUTION_OFFSET = 1u;
const uint8_t CALIBRATION_OFFSET = 3u;
const uint8_t RUN_OFFSET = 4u;
const uint8_t IO0_DATA_OFFSET = 0u;
const uint8_t GG_RESET_OFFSET = 1u;
const uint8_t GG_EOC_OFFSET = 2u;
const uint8_t VTM_EOC_OFFSET = 3u;
const uint8_t POR_RESET_OFFSET = 4u;

const uint8_t EXTERNAL_CLOCK_MASK = 0x01;
const uint8_t RESOLUTION_MASK = 0x06;
const uint8_t CALIBRATION_MASK = 0x08;
const uint8_t RUN_MASK = 0x10;
const uint8_t IO0_DATA_MASK = 0x01;
const uint8_t GG_RESET_MASK = 0x01;
const uint8_t GG_EOC_MASK = 0x04;
const uint8_t VTM_EOC_MASK = 0x08;
const uint8_t POR_RESET_MASK = 0x10;


static void voltageInit();
static void setDefaultStcRegisterValues();
static void setModeRegister(ExternalClockOption externalClockOption,
							ResolutionOption resolutionOption,
							CalibrationOption calibrationOption,
							RunOption runOption);
static void setCtrlRegister(IO0DataOption io0DataOption,
							GGResetOption ggResetOption,
							GgEocOption ggEocOption,
							VtmEocOption vtmEocOption,
							PorResetOption porResetOption);
static void setChargeLowRegister(uint8_t chargeLow);
static void setChargeHighRegister(uint8_t chargeHigh);
static void setCounterLowRegister(uint8_t counterLow);
static void setCounterHighRegister(uint8_t counterHigh);
static void setCurrentLowRegister(uint8_t currentLow);
static void setCurrentHighRegister(uint8_t currentHigh);
static void setVoltageLowRegister(uint8_t voltageLow);
static void setVoltageHighRegister(uint8_t voltageHigh);
static void setTemperatureLowRegister(uint8_t temperatureLow);
static void setTemperatureHighRegister(uint8_t temperatureHigh);
static void setId0Register(uint8_t io0);
static void setId1Register(uint8_t io1);
static void setId2Register(uint8_t io2);
static void setId3Register(uint8_t io3);
static void setId4Register(uint8_t io4);
static void setId5Register(uint8_t io5);
static void setId6Register(uint8_t io6);
static void setId7Register(uint8_t io7);
static ExternalClockOption getExternalClock();
static ResolutionOption getResolution();
static CalibrationOption getCalibration();
static RunOption getRun();
static IO0DataOption getIo0Data();
static GGResetOption getGgReset();
static GgEocOption getGgEoc();
static VtmEocOption getVtmOption();
static PorResetOption getPorReset();
static uint8_t getChargeLow();
static uint8_t getChargeHigh();
static uint8_t getCounterLow();
static uint8_t getCounterHigh();
static uint8_t getCurrentLow();
static uint8_t getCurrentHigh();
static uint8_t getVoltageLow();
static uint8_t getVoltageHigh();
static uint8_t getTemperatureLow();
static uint8_t getTemperatureHigh();
static uint8_t getId0();
static uint8_t getId1();
static uint8_t getId2();
static uint8_t getId3();
static uint8_t getId4();
static uint8_t getId5();
static uint8_t getId6();
static uint8_t getId7();
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

		osDelayUntil(tick + pdMS_TO_TICKS(4000));
	}
}

static void voltageInit()
{
	HAL_StatusTypeDef status = HAL_ERROR;
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	do
	{
		status = HAL_I2C_IsDeviceReady(&hi2c1, STC_ADDRESS << 1, 1, 1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	setDefaultStcRegisterValues();
	setModeRegister(AUTO_DETECT,
					RESOLUTION_14_BITS,
					NO_CALIBRATION,
					OPERATING_MODE);
	status = HAL_ERROR;
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	do
	{
		status = HAL_I2C_Mem_Write(&hi2c1,
								   STC_ADDRESS << 1,
								   MODE_ADDRESS, I2C_MEMADD_SIZE_8BIT,
								   stcRegisters + MODE_INDEX, 1,
								   1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);

	//TODO имплементировать коллбеки. Нон блокинг через прерывания. В коллбеке выдача разрешения/семафора на дальнейшую работу. задачу-вызов определять по адресу буфера
}

static void setDefaultStcRegisterValues()
{
	setModeRegister(AUTO_DETECT,
					RESOLUTION_14_BITS,
					NO_CALIBRATION,
					STANDBY_MODE);
	setCtrlRegister(IO0_HIGH,
					GG_NO_RESET,
					GG_END_OF_CONVERSION,
					VTM_END_OF_CONVERSION,
					POR_NO_RESET);
	setChargeLowRegister(0);
	setChargeHighRegister(0);
	setCounterLowRegister(0);
	setCounterHighRegister(0);
	setCurrentLowRegister(0);
	setCurrentHighRegister(0);
	setVoltageLowRegister(0);
	setVoltageHighRegister(0);
	setTemperatureLowRegister(0);
	setTemperatureHighRegister(0);
	setId0Register(PART_TYPE_ID);
	setId1Register(0);
	setId2Register(0);
	setId3Register(0);
	setId4Register(0);
	setId5Register(0);
	setId6Register(0);
	setId7Register(0);
}

static void setModeRegister(ExternalClockOption externalClockOption,
							ResolutionOption resolutionOption,
							CalibrationOption calibrationOption,
							RunOption runOption)
{
	uint8_t reg = 0;
	reg |= (externalClockOption << EXTERNAL_CLOCK_OFFSET);
	reg |= (resolutionOption << RESOLUTION_OFFSET);
	reg |= (calibrationOption << CALIBRATION_OFFSET);
	reg |= (runOption << RUN_OFFSET);
	stcRegisters[MODE_INDEX] = reg;
}

static void setCtrlRegister(IO0DataOption io0DataOption,
							GGResetOption ggResetOption,
							GgEocOption ggEocOption,
							VtmEocOption vtmEocOption,
							PorResetOption porResetOption)
{
	uint8_t reg = 0;
	reg |= (io0DataOption << IO0_DATA_OFFSET);
	reg |= (ggResetOption << GG_RESET_OFFSET);
	reg |= (ggEocOption << GG_EOC_OFFSET);
	reg |= (vtmEocOption << VTM_EOC_OFFSET);
	reg |= (porResetOption << POR_RESET_OFFSET);
	stcRegisters[CTRL_INDEX] = reg;
}

static void setChargeLowRegister(uint8_t chargeLow)
{
	stcRegisters[CHARGE_LOW_INDEX] = chargeLow;
}

static void setChargeHighRegister(uint8_t chargeHigh)
{
	stcRegisters[CHARGE_HIGH_INDEX] = chargeHigh;
}

static void setCounterLowRegister(uint8_t counterLow)
{
	stcRegisters[COUNTER_LOW_INDEX] = counterLow;
}

static void setCounterHighRegister(uint8_t counterHigh)
{
	stcRegisters[COUNTER_HIGH_INDEX] = counterHigh;
}

static void setCurrentLowRegister(uint8_t currentLow)
{
	stcRegisters[CURRENT_LOW_INDEX] = currentLow;
}

static void setCurrentHighRegister(uint8_t currentHigh)
{
	stcRegisters[CURRENT_HIGH_INDEX] = currentHigh;
}

static void setVoltageLowRegister(uint8_t voltageLow)
{
	stcRegisters[VOLTAGE_LOW_INDEX] = voltageLow;
}

static void setVoltageHighRegister(uint8_t voltageHigh)
{
	stcRegisters[VOLTAGE_HIGH_INDEX] = voltageHigh;
}

static void setTemperatureLowRegister(uint8_t temperatureLow)
{
	stcRegisters[TEMPERATURE_LOW_INDEX] = temperatureLow;
}

static void setTemperatureHighRegister(uint8_t temperatureHigh)
{
	stcRegisters[TEMPERATURE_HIGH_INDEX] = temperatureHigh;
}

static void setId0Register(uint8_t io0)
{
	stcRegisters[ID0_INDEX] = io0;
}

static void setId1Register(uint8_t io1)
{
	stcRegisters[ID1_INDEX] = io1;
}

static void setId2Register(uint8_t io2)
{
	stcRegisters[ID2_INDEX] = io2;
}

static void setId3Register(uint8_t io3)
{
	stcRegisters[ID3_INDEX] = io3;
}

static void setId4Register(uint8_t io4)
{
	stcRegisters[ID4_INDEX] = io4;
}

static void setId5Register(uint8_t io5)
{
	stcRegisters[ID5_INDEX] = io5;
}

static void setId6Register(uint8_t io6)
{
	stcRegisters[ID6_INDEX] = io6;
}

static void setId7Register(uint8_t io7)
{
	stcRegisters[ID7_INDEX] = io7;
}

//TODO обобщить геттеры

static ExternalClockOption getExternalClock()
{
	uint8_t reg = stcRegisters[MODE_INDEX];
	ExternalClockOption externalClockOption = (ExternalClockOption)
			((reg & EXTERNAL_CLOCK_MASK) >> EXTERNAL_CLOCK_OFFSET);
	return externalClockOption;
}

static ResolutionOption getResolution()
{
	uint8_t reg = stcRegisters[MODE_INDEX];
	ResolutionOption resolutionOption = (ResolutionOption)
			((reg & RESOLUTION_MASK) >> RESOLUTION_OFFSET);
	return resolutionOption;
}

static CalibrationOption getCalibration()
{
	uint8_t reg = stcRegisters[MODE_INDEX];
	CalibrationOption calibrationOption = (CalibrationOption)
			((reg & CALIBRATION_MASK) >> CALIBRATION_OFFSET);
	return calibrationOption;
}

static RunOption getRun()
{
	uint8_t reg = stcRegisters[MODE_INDEX];
	RunOption runOption = (RunOption)
			((reg & RUN_MASK) >> RUN_OFFSET);
	return runOption;
}

static IO0DataOption getIo0Data()
{
	uint8_t reg = stcRegisters[CTRL_INDEX];
	IO0DataOption io0DataOption = (IO0DataOption)
			((reg & IO0_DATA_MASK) >> IO0_DATA_OFFSET);
	return io0DataOption;
}

static GGResetOption getGgReset()
{
	uint8_t reg = stcRegisters[CTRL_INDEX];
	GGResetOption ggResetOption = (GGResetOption)
			((reg & GG_RESET_MASK) >> GG_RESET_OFFSET);
	return ggResetOption;
}

static GgEocOption getGgEoc()
{
	uint8_t reg = stcRegisters[CTRL_INDEX];
	GgEocOption ggEocOption = (GgEocOption)
			((reg & GG_EOC_MASK) >> GG_EOC_OFFSET);
	return ggEocOption;
}

static VtmEocOption getVtmOption()
{
	uint8_t reg = stcRegisters[CTRL_INDEX];
	VtmEocOption vtmEocOption = (VtmEocOption)
			((reg & VTM_EOC_MASK) >> VTM_EOC_OFFSET);
	return vtmEocOption;
}

static PorResetOption getPorReset()
{
	uint8_t reg = stcRegisters[CTRL_INDEX];
	PorResetOption porResetOption = (PorResetOption)
			((reg & POR_RESET_MASK) >> POR_RESET_OFFSET);
	return porResetOption;
}

static uint8_t getChargeLow()
{
	return stcRegisters[CHARGE_LOW_INDEX];
}

static uint8_t getChargeHigh()
{
	return stcRegisters[CHARGE_HIGH_INDEX];
}

static uint8_t getCounterLow()
{
	return stcRegisters[COUNTER_LOW_INDEX];
}

static uint8_t getCounterHigh()
{
	return stcRegisters[COUNTER_HIGH_INDEX];
}

static uint8_t getCurrentLow()
{
	return stcRegisters[CURRENT_LOW_INDEX];
}

static uint8_t getCurrentHigh()
{
	return stcRegisters[CURRENT_HIGH_INDEX];
}

static uint8_t getVoltageLow()
{
	return stcRegisters[VOLTAGE_LOW_INDEX];
}

static uint8_t getVoltageHigh()
{
	return stcRegisters[VOLTAGE_HIGH_INDEX];
}

static uint8_t getTemperatureLow()
{
	return stcRegisters[TEMPERATURE_LOW_INDEX];
}

static uint8_t getTemperatureHigh()
{
	return stcRegisters[TEMPERATURE_HIGH_INDEX];
}

static uint8_t getId0()
{
	return stcRegisters[ID0_INDEX];
}

static uint8_t getId1()
{
	return stcRegisters[ID1_INDEX];
}

static uint8_t getId2()
{
	return stcRegisters[ID2_INDEX];
}

static uint8_t getId3()
{
	return stcRegisters[ID3_INDEX];
}

static uint8_t getId4()
{
	return stcRegisters[ID4_INDEX];
}

static uint8_t getId5()
{
	return stcRegisters[ID5_INDEX];
}

static uint8_t getId6()
{
	return stcRegisters[ID6_INDEX];
}

static uint8_t getId7()
{
	return stcRegisters[ID7_INDEX];
}

static void readVoltageData()
{
	HAL_StatusTypeDef status = HAL_ERROR;
	osMutexAcquire(mutexI2cHandle, osWaitForever);
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c1,
								  STC_ADDRESS << 1,
								  VOLTAGE_LOW_ADDRESS, I2C_MEMADD_SIZE_8BIT,
								  stcRegisters + VOLTAGE_LOW_INDEX, 2,
								  1000);
	}while(status != HAL_OK);
	osMutexRelease(mutexI2cHandle);
}

static uint32_t calculateVoltage()
{
	static uint32_t voltage_ = 0;

	uint8_t voltageHigh = stcRegisters[VOLTAGE_HIGH_INDEX];
	uint8_t voltageLow = stcRegisters[VOLTAGE_LOW_INDEX];
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
