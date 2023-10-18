#include "lux.h"
#include "i2c.h"
#include "main.h"


#define LTR_REGISTERS_SIZE 9u


volatile uint8_t ltrRegisters[LTR_REGISTERS_SIZE];
const uint8_t ALS_CONTR_INDEX	   	 = 0u;
const uint8_t ALS_MEAS_INDEX	   	 = 1u;
const uint8_t PART_ID_INDEX		   	 = 2u;
const uint8_t MANUFAC_ID_INDEX	  	 = 3u;
const uint8_t ALS_DATA_CH1_0_INDEX	 = 4u;
const uint8_t ALS_DATA_CH1_1_INDEX	 = 5u;
const uint8_t ALS_DATA_CH0_0_INDEX	 = 6u;
const uint8_t ALS_DATA_CH0_1_INDEX	 = 7u;
const uint8_t ALS_STATUS_INDEX	   	 = 8u;

const uint8_t LTR_ADDRESS			 = 0x29;
const uint8_t ALS_CONTR_ADDRESS		 = 0x80;
const uint8_t ALS_MEAS_ADDRESS		 = 0x85;
const uint8_t PART_ID_ADDRESS		 = 0x86;
const uint8_t MANUFAC_ID_ADDRESS	 = 0x87;
const uint8_t ALS_DATA_CH1_0_ADDRESS = 0x88;
const uint8_t ALS_DATA_CH1_1_ADDRESS = 0x89;
const uint8_t ALS_DATA_CH0_0_ADDRESS = 0x8A;
const uint8_t ALS_DATA_CH0_1_ADDRESS = 0x8B;
const uint8_t ALS_STATUS_ADDRESS	 = 0x8C;


void luxInit()
{
	//Wait 100 ms (min) - initial startup time
	//I2C Command (Write) To enable sensor to Active Mode
	//Wait 10 ms (max) - wakeup time from standby
	//Считать id регистры

	for(uint8_t i = 0; i < LTR_REGISTERS_SIZE; ++i)
	{
		ltrRegisters[i] = 0;
	}




	for(uint32_t i = 0; i < 500000; ++i){}
	uint8_t bufToWrite = 0x19;
	HAL_StatusTypeDef stat1 = HAL_I2C_Mem_Write(&hi2c1, LTR_ADDRESS<<1, ALS_CONTR_ADDRESS, 1, &bufToWrite, 1, 5000);


	for(uint32_t i = 0; i < 500000; ++i){}
	uint8_t bufToRead[2];
	bufToRead[0] = 0;
	bufToRead[1] = 0;
	HAL_StatusTypeDef stat2 = HAL_I2C_Mem_Read(&hi2c1, LTR_ADDRESS<<1, PART_ID_ADDRESS, 1, bufToRead, 2, 5000);

	for(uint32_t i = 0; i < 500000; ++i){}

	//TODO имплементировать коллбеки. Нон блокинг через прерывания. В коллбеке выдача разрешения/семафора на дальнейшую работу. задачу-вызов определять по адресу буфера
}
