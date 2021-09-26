/*
 * TM1638.c
 *
 *  Created on: Sep 17, 2021
 *      Author: user
 */
#include <memory.h>
#include <stdio.h>
#include "TM1638.h"


extern SPI_HandleTypeDef hspi2;
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
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
  };

#define CS_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);//(GPIOB->BSRR = ((uint16_t)0x1000))
#define CS_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);//(GPIOB->BRR = ((uint16_t)0x1000))

uint8_t tm1638_buff[16];
uint8_t  tm1638_command;
uint8_t  tm1638_addr;
uint8_t  tm1638_keys;
SPI_HandleTypeDef *pSPI=&hspi2;

void TM1638WriteBuff();

void TM1638_Init(){
	tm1638_keys=0;
	memset(tm1638_buff,0x00,16);
	tm1638_command=0x80;
	CS_L;
	HAL_SPI_Transmit(pSPI,&tm1638_command,1,500);
	CS_H;
	TM1638WriteBuff();
};

void TM1638_Led(uint8_t num, uint8_t value){
	if(num>7)
		return;
	tm1638_buff[1+num*2]=value?0x0f:0;
};
void TM1638_Char(uint8_t num, uint8_t value){
	if(num>7)
		return;
	tm1638_buff[num*2]=value;
};
void TM1638_Update(){
	tm1638_command=0x80;
	CS_L;
	HAL_SPI_Transmit(pSPI,&tm1638_command,1,500);
	CS_H;
	TM1638WriteBuff();
};

void TM1638WriteBuff(){
	tm1638_command=0x40;
	tm1638_addr=0xc0;
	CS_L;
	HAL_SPI_Transmit(pSPI,&tm1638_command,1,500);
	CS_H;

	CS_L;
	HAL_SPI_Transmit(pSPI,&tm1638_addr,1,500);
	HAL_SPI_Transmit(pSPI,tm1638_buff,16,500);

	CS_H;



};
uint16_t TM1638_ReadKey(){
	uint8_t keys=0;
	uint8_t data[4];
	tm1638_command=0x42;
	CS_L;
	HAL_SPI_Transmit(pSPI,&tm1638_command,1,500);
	pSPI->Instance->CR1 &= ~SPI_CR1_BIDIOE;
	HAL_SPI_Receive(pSPI,data,4,500);
	pSPI->Instance->CR1 |= SPI_CR1_BIDIOE;
	CS_H;
	for(int i=0;i<4;i++)
		keys |= (data[i]&0x11)<<i;
	if(keys==tm1638_keys){
		return 0;
	}

		data[0]=(keys^tm1638_keys)&keys;
		data[1]=(keys^tm1638_keys)&tm1638_keys;
	    tm1638_keys=keys;
	    return (data[1]<<8)|data[0];


};
