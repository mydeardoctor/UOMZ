#include "leds.h"
#include "main.h"

void ledsHandler(void)
{
//	HAL_GPIO_WritePin(SPO2_OE_GPIO_Port, SPO2_OE_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(BAR_L_OE_GPIO_Port, BAR_L_OE_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(BAR_R_OE_GPIO_Port, BAR_R_OE_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(MIXLED_OE_GPIO_Port, MIXLED_OE_Pin, GPIO_PIN_SET);

	//BAR_R_7
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_5
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_8
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_3
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_4
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_1
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_2
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_R_6
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//S_-1-.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_-1-.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//S_1--.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_1--.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//S_--1.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//S_--1.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//MUTE
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//MOS_BLU
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//MOS_GREEN
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//MOS_RED
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//CHG_BLU
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//CHG_GRN
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//CHG_RED
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//NC
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//BAR_L_7
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_5
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_8
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_3
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_4
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_1
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_2
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//BAR_L_6
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//HR_-1-.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_-1-.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//HR_1--.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_1--.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	//HR_--1.B
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.A
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.F
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.G
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.C
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.D
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.E
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);
	//HR_--1.H
	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_CLK_GPIO_Port, RG_CLK_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 200; ++i){}
	HAL_GPIO_WritePin(RG_STROBE_GPIO_Port, RG_STROBE_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(RG_DATA0_GPIO_Port, RG_DATA0_Pin, GPIO_PIN_RESET);
}
