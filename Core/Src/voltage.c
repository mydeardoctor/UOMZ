#include <stdbool.h>
#include "cmsis_os.h"


volatile uint16_t voltage = 0u;
extern osMutexId_t mutexVoltageHandle;


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
