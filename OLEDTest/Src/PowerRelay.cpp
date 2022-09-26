/*
 * PowerRelay.cpp
 *
 *  Created on: Sep 26, 2022
 *      Author: user
 */
#include "PowerRelay.h"
#include "TM1638.h"
#include "ds18b20.h"
extern TIM_HandleTypeDef htim1;
uint8_t g_PowerState=0;
uint16_t g_StopTemp=990;
void PowerRelayOn(){
   g_PowerState=1;
   TM1638_Led(1, 0);
   TM1638_Led(7, 1);
   TM1638_Update();
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
};
void PowerRelayOff(){
	g_PowerState=g_PowerState&0xfe;
	TM1638_Led(7, 0);
	TM1638_Update();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
};
uint8_t PowerRelayCheck(){
	return g_PowerState;
};
void PowerRelayLimitCheck(){
	if( g_PowerState){
		if((ds18_sensors[dev_index[/*2*/0]].bError)||(g_StopTemp<(ds18_sensors[dev_index[/*2*/0]].temp*10.0))){
		  TM1638_Led(1, 1);
		  g_PowerState=g_PowerState|0x2;
		  PowerRelayOff();
		  HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
		}
	}
};

