/*
 * TM1638.h
 *
 *  Created on: Sep 17, 2021
 *      Author: user
 */

#ifndef TM1638_H_
#define TM1638_H_
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void TM1638_Init();
void TM1638_Led(uint8_t num, uint8_t value);
void TM1638_Update();
uint16_t TM1638_ReadKey();
extern uint8_t  tm1638_keys;


#endif /* TM1638_H_ */
