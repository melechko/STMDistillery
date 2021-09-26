/*
 * MainLoop.cpp
 *
 *  Created on: Sep 26, 2021
 *      Author: user
 */

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Screen.h"
#include "fonts.h"
#include "ssd1306.h"
#include "BME280.h"
#include "TM1638.h"
#include "test.h"
#include "bitmap.h"
#include "onewire.h"
#include "MainLoop.h"

CScreen *gScreen=NULL;
void MainLoopInit(){
	gScreen= new CStartScreen();
	TM1638_Init();
	gScreen->Init();
};
void MainLoopRun(){
	uint16_t keys;
	keys = TM1638_ReadKey();
		if (keys) {
			if (gScreen) {
				CScreen *pScreen = gScreen->ProcessKey(keys);
				if (pScreen) {
					if (gScreen) {
						delete gScreen;
						gScreen = NULL;
					}
					gScreen = pScreen;
					gScreen->Init();
				}
			}

		}


	if (timer1 > 800) {
		if (owdevices) {
			for (uint8_t i = 0; i < owdevices; i++)
				ds18b20_get_temp(i);
			ds18b20_start_convert();
			/*if (owdevices) {
				sprintf(str1, "%4.2f ", ds18_sensors[0].temp);
				SSD1306_GotoXY(0, 0 * 20);
				SSD1306_Puts(str1, &Font_11x18, SSD1306_COLOR_WHITE);
				if (owdevices > 1) {
					sprintf(str1, "%4.2f ", ds18_sensors[1].temp);
					SSD1306_GotoXY(64, 0 * 20);
					SSD1306_Puts(str1, &Font_11x18, SSD1306_COLOR_WHITE);
					if (owdevices > 2) {
						sprintf(str1, "%4.2f ", ds18_sensors[2].temp);
						SSD1306_GotoXY(0, 1 * 20);
						SSD1306_Puts(str1, &Font_11x18, SSD1306_COLOR_WHITE);
						if (owdevices > 3) {
							sprintf(str1, "%4.2f ", ds18_sensors[3].temp);
							SSD1306_GotoXY(64, 1 * 20);
							SSD1306_Puts(str1, &Font_11x18, SSD1306_COLOR_WHITE);
						}
					}
				}
			}*/
		}
		/*sprintf(str1, "%.2f'C %.2fmm    ", BME280_ReadTemperature(),
				BME280_ReadPressure() * 0.000750061683f);
		SSD1306_GotoXY(0, 2 * 20);
		SSD1306_Puts(str1, &Font_7x10, SSD1306_COLOR_WHITE);
		sprintf(str1, "%.1f%%     ", BME280_ReadHumidity());
		SSD1306_GotoXY(0, 2 * 20 + 10);
		SSD1306_Puts(str1, &Font_7x10, SSD1306_COLOR_WHITE);
		  SSD1306_UpdateScreen();*/
		timer1 = 0;
		if(gScreen)
		  gScreen->Update(1);
	} else {
		if(gScreen)
		  gScreen->Update(0);
	}
	HAL_Delay(30);



};



