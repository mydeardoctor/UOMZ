#include "lux.h"
#include "i2c.h"
#include "main.h"


#define LTR_REGISTERS_SIZE 9u

//TODO Проверить
typedef enum
{
	STAND_BY_MODE = 0x0,
	ACTIVE_MODE	  = 0x1
} ModeOption;

typedef enum
{
	SW_NO_RESET = 0x0,
	SW_RESET	= 0x1
} SwResetOption;

typedef enum
{
	GAIN_1X  = 0x0,
	GAIN_2X  = 0x1,
	GAIN_4X  = 0x2,
	GAIN_8X  = 0x3,
	GAIN_48X = 0x6,
	GAIN_96X = 0x7
} GainOption;

typedef enum
{
	MEASUREMENT_RATE_50MS	= 0x0,
	MEASUREMENT_RATE_100MS	= 0x1,
	MEASUREMENT_RATE_200MS	= 0x2,
	MEASUREMENT_RATE_500MS	= 0x3,
	MEASUREMENT_RATE_1000MS = 0x4,
	MEASUREMENT_RATE_2000MS = 0x5
} MeasurementRateOption;

typedef enum
{
	INTEGRATION_TIME_100MS	= 0x0,
	INTEGRATION_TIME_50MS	= 0x1,
	INTEGRATION_TIME_200MS	= 0x2,
	INTEGRATION_TIME_400MS	= 0x3,
	INTEGRATIONG_TIME_150MS = 0x4,
	INTEGRATION_TIME_250MS	= 0x5,
	INTEGRATION_TIME_300MS	= 0x6,
	INTEGRATION_TIME_350MS	= 0x7
} IntegrationTimeOption;

typedef enum
{
	OLD_DATA = 0x0,
	NEW_DATA = 0x1
} DataStatusOption;

typedef enum
{
	MEASURED_IN_GAIN_1X	 = 0x0,
	MEASURED_IN_GAIN_2X	 = 0x1,
	MEASURED_IN_GAIN_4X	 = 0x2,
	MEASURED_IN_GAIN_8X	 = 0x3,
	MEASURED_IN_GAIN_48X = 0x6,
	MEASURED_IN_GAIN_96X = 0x7
} DataGainRangeOption;

typedef enum
{
	DATA_IS_VALID	= 0x0,
	DATA_IS_INVALID = 0x1
} DataValidOption;


volatile uint8_t ltrRegisters[LTR_REGISTERS_SIZE];
const uint8_t CONTR_INDEX	   	 = 0u;
const uint8_t MEAS_RATE_INDEX	 = 1u;
const uint8_t PART_ID_INDEX		 = 2u;
const uint8_t MANUFAC_ID_INDEX	 = 3u;
const uint8_t DATA_CH1_0_INDEX	 = 4u;
const uint8_t DATA_CH1_1_INDEX	 = 5u;
const uint8_t DATA_CH0_0_INDEX	 = 6u;
const uint8_t DATA_CH0_1_INDEX	 = 7u;
const uint8_t STATUS_INDEX	   	 = 8u;

const uint8_t LTR_ADDRESS		 = 0x29;
const uint8_t CONTR_ADDRESS		 = 0x80;
const uint8_t MEAS_RATE_ADDRESS	 = 0x85;
const uint8_t PART_ID_ADDRESS	 = 0x86;
const uint8_t MANUFAC_ID_ADDRESS = 0x87;
const uint8_t DATA_CH1_0_ADDRESS = 0x88;
const uint8_t DATA_CH1_1_ADDRESS = 0x89;
const uint8_t DATA_CH0_0_ADDRESS = 0x8A;
const uint8_t DATA_CH0_1_ADDRESS = 0x8B;
const uint8_t STATUS_ADDRESS	 = 0x8C;

const uint8_t PART_NUMBER_ID	 = 0x0A;
const uint8_t REVISION_ID		 = 0x0;
const uint8_t MANUFACTURER_ID	 = 0x05;

const uint8_t MODE_OFFSET			  = 0u;
const uint8_t SW_RESET_OFFSET		  = 1u;
const uint8_t GAIN_OFFSET			  = 2u;
const uint8_t MEASUREMENT_RATE_OFFSET = 0u;
const uint8_t INTEGRATION_TIME_OFFSET = 3u;
const uint8_t REVISION_ID_OFFSET	  = 0u;
const uint8_t PART_NUMBER_ID_OFFSET	  = 4u;
const uint8_t DATA_STATUS_OFFSET	  = 2u;
const uint8_t DATA_GAIN_RANGE_OFFSET  = 4u;
const uint8_t DATA_VALID_OFFSET		  = 7u;

const uint8_t REVISION_ID_MASK		  = 0x0F;
const uint8_t PART_NUMBER_ID_MASK	  = 0xF0;
const uint8_t DATA_STATUS_MASK		  = 0x04;
const uint8_t DATA_GAIN_RANGE_MASK	  = 0x70;
const uint8_t DATA_VALID_MASK		  = 0x80;


static void setDefaultLtrRegisterValues();
static void setContrRegister(ModeOption modeOption,
							 SwResetOption swResetOption,
							 GainOption gainOption);
static void setMeasRateRegister(MeasurementRateOption measurementRateOption,
								IntegrationTimeOption integrationTimeOption);
static void setPartIdRegister();
static void setManufacIdRegister();
static void setDataCh1LowRegister(uint8_t dataCh1Low);
static void setDataCh1HighRegister(uint8_t dataCh1High);
static void setDataCh0LowRegister(uint8_t dataCh0Low);
static void setDataCh0HighRegister(uint8_t dataCh0High);
static void setStatusRegister(DataStatusOption dataStatusOption,
							  DataGainRangeOption dataGainRangeOption,
							  DataValidOption dataValidOption);


//TODO Спрятать низкоуровневые дедали в функции.
//Функции инициализации каждого регистра
//Начальные значения
void luxInit()
{
	setDefaultLtrRegisterValues();


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
	HAL_StatusTypeDef stat1 = HAL_I2C_Mem_Write(&hi2c1,
												LTR_ADDRESS<<1,
												CONTR_ADDRESS, 1,
												&bufToWrite, 1,
												5000);


	for(uint32_t i = 0; i < 500000; ++i){}
	uint8_t bufToRead[2];
	bufToRead[0] = 0;
	bufToRead[1] = 0;
	HAL_StatusTypeDef stat2 = HAL_I2C_Mem_Read(&hi2c1,
											   LTR_ADDRESS<<1,
											   PART_ID_ADDRESS, 1,
											   bufToRead, 2,
											   5000);

	for(uint32_t i = 0; i < 500000; ++i){}

	//TODO имплементировать коллбеки. Нон блокинг через прерывания. В коллбеке выдача разрешения/семафора на дальнейшую работу. задачу-вызов определять по адресу буфера
}



//TODO отладить
static void setDefaultLtrRegisterValues()
{
	setContrRegister(STAND_BY_MODE, SW_NO_RESET, GAIN_1X);
	setMeasRateRegister(MEASUREMENT_RATE_500MS, INTEGRATION_TIME_100MS);
	setPartIdRegister();
	setManufacIdRegister();
	setDataCh1LowRegister(0);
	setDataCh1HighRegister(0);
	setDataCh0LowRegister(0);
	setDataCh0HighRegister(0);
	setStatusRegister(OLD_DATA, MEASURED_IN_GAIN_1X, DATA_IS_VALID);
}

static void setContrRegister(ModeOption modeOption,
							 SwResetOption swResetOption,
							 GainOption gainOption)
{
	uint8_t reg = 0;
	reg |= (modeOption << MODE_OFFSET);
	reg |= (swResetOption << SW_RESET_OFFSET);
	reg |= (gainOption << GAIN_OFFSET);
	ltrRegisters[CONTR_INDEX] = reg;
}

static void setMeasRateRegister(MeasurementRateOption measurementRateOption,
								IntegrationTimeOption integrationTimeOption)
{
	uint8_t reg = 0;
	reg |= (measurementRateOption << MEASUREMENT_RATE_OFFSET);
	reg |= (integrationTimeOption << INTEGRATION_TIME_OFFSET);
	ltrRegisters[MEAS_RATE_INDEX] = reg;
}

static void setPartIdRegister()
{
	uint8_t reg = 0;
	reg |= (REVISION_ID << REVISION_ID_OFFSET);
	reg |= (PART_NUMBER_ID << PART_NUMBER_ID_OFFSET);
	ltrRegisters[PART_ID_INDEX] = reg;
}

static void setManufacIdRegister()
{
	ltrRegisters[MANUFAC_ID_INDEX] = MANUFACTURER_ID;
}

static void setDataCh1LowRegister(uint8_t dataCh1Low)
{
	ltrRegisters[DATA_CH1_0_INDEX] = dataCh1Low;
}

static void setDataCh1HighRegister(uint8_t dataCh1High)
{
	ltrRegisters[DATA_CH1_1_INDEX] = dataCh1High;
}

static void setDataCh0LowRegister(uint8_t dataCh0Low)
{
	ltrRegisters[DATA_CH0_0_INDEX] = dataCh0Low;
}

static void setDataCh0HighRegister(uint8_t dataCh0High)
{
	ltrRegisters[DATA_CH0_1_INDEX] = dataCh0High;
}

static void setStatusRegister(DataStatusOption dataStatusOption,
							  DataGainRangeOption dataGainRangeOption,
							  DataValidOption dataValidOption)
{
	uint8_t reg = 0;
	reg |= (dataStatusOption << DATA_STATUS_OFFSET);
	reg |= (dataGainRangeOption << DATA_GAIN_RANGE_OFFSET);
	reg |= (dataValidOption << DATA_VALID_OFFSET);
	ltrRegisters[STATUS_INDEX] = reg;
}

//TODO геттеры

//static uin16_t calculateLux(channel0Msb, channel0Lsb, channel1Msb, channel1Lsb)
//{
//	uint16_t lux = 0;
//
//
//
//	return lux;
//}
