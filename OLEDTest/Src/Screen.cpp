/*
 * Scren.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: user
 */


#include "Screen.h"
#include "ssd1306.h"
#include "TM1638.h"

//----------------------------------
char cMoonShine[]="MoonShine";
char cVersion[]="v0.1";
char cPressAnyKey[]=     "Press any key";
char cPressAnyKeyBlank[]="             ";
//----------------------------------

CStartScreen::~CStartScreen(){

};
CScreen * CStartScreen::ProcessKey(uint16_t keys){
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
void CStartScreen::Update() {
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
}
;
