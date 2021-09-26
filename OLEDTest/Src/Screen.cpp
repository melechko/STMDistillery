/*
 * Scren.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: user
 */


#include "Screen.h"
#include "ssd1306.h"
#include "TM1638.h"
#include "ds18b20.h"
#include <cstdio>

void CScreen::DisplayLedTEMP() {
	if (dev_index[2] != 255) {
		for (int i = 0; i < 4; i++)
			TM1638_Char(i, (ds18_sensors[dev_index[2]].itemp >> (8 * (3 - i))) & 0xff);
	} else
		for (int i = 0; i < 4; i++)
			TM1638_Char(i, digitToSegment[0x10]);
	if (dev_index[0] != 255) {
		for (int i = 0; i < 4; i++)
			TM1638_Char(i + 4, (ds18_sensors[dev_index[0]].itemp >> (8 * (3 - i))) & 0xff);
	} else
		for (int i = 0; i < 4; i++)
			TM1638_Char(i + 4, digitToSegment[0x10]);
	TM1638_Update();

}
;
//----------------------------------
char cMoonShine[]="MoonShine";
char cVersion[]="v0.1";
char cPressAnyKey[]=     "Press any key";
char cPressAnyKeyBlank[]="             ";
#define MENU_SIZE 5
char acMenu[5][9]={{"Start   "},{"Setup   "},{"Info    "},{"Menu 1  "},{"Menu 2  "}};
char cDeviceNotFound[]="Device not found";
//----------------------------------

CStartScreen::~CStartScreen(){

};
CScreen * CStartScreen::ProcessKey(uint16_t keys){
	if(keys&0xff00){
		return new CMenuScreen();
	}
  return NULL;
};
void CStartScreen::Init(){
	m_count=0;
	SSD1306_DrawRectangle(0, 0, 127, 63, SSD1306_COLOR_WHITE);
	SSD1306_DrawFilledRectangle(8, 8, 127-16, 63-16, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY (16,9);
	SSD1306_Puts (cMoonShine, &Font_11x18, SSD1306_COLOR_BLACK);
	SSD1306_GotoXY (44,27);
	SSD1306_Puts (cVersion, &Font_11x18, SSD1306_COLOR_BLACK);
	SSD1306_UpdateScreen();
    for(int i=0;i<8;i++)
    	TM1638_Char(i,digitToSegment[0x10]);
    TM1638_Update();

};
void CStartScreen::Update(uint8_t bNew) {
	m_count++;
	if (m_count == 32) {
		SSD1306_GotoXY(20, 45);
		SSD1306_Puts(cPressAnyKey, &Font_7x10, SSD1306_COLOR_BLACK);
		SSD1306_UpdateScreen();
	} else {
		if (m_count >= 64) {
			SSD1306_GotoXY(20, 45);
			SSD1306_Puts(cPressAnyKeyBlank, &Font_7x10, SSD1306_COLOR_BLACK);
			SSD1306_UpdateScreen();
			m_count = 0;
		}
	}
};

CMenuScreen::~CMenuScreen(){

};
uint8_t CMenuScreen::m_curr=0;
uint8_t CMenuScreen::m_start=0;
CScreen* CMenuScreen::ProcessKey(uint16_t keys) {
	uint8_t b = 0;
	if (keys & 0x100) {
		if (m_curr) {
			m_curr--;
			if (m_curr < m_start)
				m_start--;
			b = 1;
		}

	}
	if (keys & 0x200) {
		if (m_curr < MENU_SIZE - 1) {
			m_curr++;
			if (m_curr >= m_start + 3)
				m_start++;
			b = 1;
		}
	}
	if (b) {
		DrawMenu();
	}
	if(keys & 0x1000){
		switch(m_curr){
		case 0:{

		}
		break;
		case 1:{

		}
		break;
		case 2:{
			return  new CInfoScreen();

		}
		break;
		case 3:{

		}
		break;
		case 4:{

		}
		break;

		}
	}
	if(keys & 0x20)
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	if(keys & 0x2000)
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	return NULL;
}
;
void CMenuScreen::Init(){
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_DrawRectangle(0, 0, 127, 63, SSD1306_COLOR_WHITE);
//    m_start=0;
//    m_curr=0;
    DrawMenu();
};
void CMenuScreen::Update(uint8_t bNew){
	if(bNew)
		DisplayLedTEMP();

};
void CMenuScreen::DrawMenu() {
	for (int i = 0; i < 3; i++) {
		SSD1306_GotoXY(2, 2 + 20 * i);
		SSD1306_Puts(acMenu[m_start + i], &Font_11x18,
				m_start + i == m_curr ?
						SSD1306_COLOR_BLACK : SSD1306_COLOR_WHITE);
	}
	SSD1306_UpdateScreen();

}
;
CInfoScreen::~CInfoScreen() {

}
;
CScreen* CInfoScreen::ProcessKey(uint16_t keys) {
	if (keys & 0xff00) {
		return new CMenuScreen();
	}
	return NULL;
};
void CInfoScreen::Init() {
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	if (owdevices) {
		char str[20];
		uint32_t p;
		for (int i = 0; i < owdevices; i++) {
			//p=(uint32_t*) ds18_sensors[i].rom_code
			p=ds18_sensors[i].rom_code[3]+(ds18_sensors[i].rom_code[2]<<8)+(ds18_sensors[i].rom_code[1]<<16)+(ds18_sensors[i].rom_code[0]<<24);
			sprintf(str, "%d)%08lX",i+1, p);
			p=ds18_sensors[i].rom_code[7]+(ds18_sensors[i].rom_code[6]<<8)+(ds18_sensors[i].rom_code[5]<<16)+(ds18_sensors[i].rom_code[4]<<24);
			sprintf(str+10, "%08lX", p);
			SSD1306_GotoXY(0, 10 * i);
			SSD1306_Puts(str, &Font_7x10, SSD1306_COLOR_WHITE);
		}

	} else {
		SSD1306_GotoXY(0, 10);
		SSD1306_Puts(cDeviceNotFound, &Font_7x10, SSD1306_COLOR_WHITE);
	}

	SSD1306_UpdateScreen();
}
;
void CInfoScreen::Update(uint8_t bNew) {
	if (bNew)
		DisplayLedTEMP();

};

CStartBeginScreen::~CStartBeginScreen(){

};
CScreen * CStartBeginScreen::ProcessKey(uint16_t keys){
	return NULL;
};
void CStartBeginScreen::Init(){

};
void CStartBeginScreen::Update(uint8_t bNew){

};
