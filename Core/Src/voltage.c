#include <stdbool.h>
#include "cmsis_os.h"


#define STC_REGISTERS_SIZE 20u


typedef enum
{
	STANDBY_MODE   = 0x0,
	OPERATING_MODE = 0x1
} RunOption;


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

//const uint8_t RUN_OFFSET = ;


static void voltageInit();
static uint16_t calculateVoltage();
static void setVoltage(uint16_t voltage_);


void taskVoltageFunction(void *argument)
{
	voltageInit();

	while(true)
	{
		uint32_t tick = osKernelGetTickCount();

		uint16_t voltage_ = calculateVoltage();
		setVoltage(voltage_);

		osDelayUntil(tick + pdMS_TO_TICKS(500));
	}
}

static void voltageInit()
{

}

static uint16_t calculateVoltage()
{
	static uint16_t voltage_ = 0;
	uint16_t temp = voltage_;

	voltage_++;
	if(voltage_ > 999)
	{
		voltage_ = 0;
	}

	return temp;
}

static void setVoltage(uint16_t voltage_)
{
	osMutexAcquire(mutexVoltageHandle, osWaitForever);
	voltage = voltage_;
	osMutexRelease(mutexVoltageHandle);
}
