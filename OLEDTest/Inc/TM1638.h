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
void TM1638_Char(uint8_t num, uint8_t value);
uint16_t TM1638_ReadKey();

extern uint8_t  tm1638_keys;

//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
extern const uint8_t digitToSegment[];/* = {
  0x3f,    // 0 0b00111111
  0x06,    // 1 0b00000110
  0x5b,    // 2 0b01011011
  0x4f,    // 3 0b01001111
  0x66,    // 4 0b01100110
  0x6d,    // 5 0b01101101
  0x7d,    // 6 0b01111101
  0x07,    // 7 0b00000111
  0x7f,    // 8 0b01111111
  0x6f,    // 9 0b01101111
  0x77,    // A 0b01110111
  0x7c,    // b 0b01111100
  0x39,    // C 0b00111001
  0x5e,    // d 0b01011110
  0x79,    // E 0b01111001
  0x71,    // F 0b01110001
  0x40,    // - 0b01000000
  0x00,     // nothing 0b00000000
  0x80     // dot
  };*/
#endif /* TM1638_H_ */
