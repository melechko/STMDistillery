#ifndef DS18B20_H_
#define DS18B20_H_
//--------------------------------------------------
#define MAX_SENSORS    5
//#include "stm32f1xx_hal.h"
//#include <string.h>
//#include <stdlib.h>
#include <stdint.h>
#include "onewire.h"
extern owdevice_t ds18_sensors[MAX_SENSORS];
extern uint8_t owdevices;
uint8_t ds18b20_start_convert();

void ds18b20_get_temp(uint8_t dev_id);
void ds18b20_init();

#endif /* DS18B20_H_ */
