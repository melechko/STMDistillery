#include "ds18b20.h"
#include <string.h>

owdevice_t ds18_sensors[MAX_SENSORS];
      //rom code
uint8_t owdevices = 0;
//uint32_t ow_tickstart=0;
//static uint8_t curr_device=0xff;//devices index

float ds18b20_tconvert(uint8_t LSB, uint8_t MSB)
{
    float data;

    uint16_t temperature;

    temperature = LSB | (MSB << 8);

	if (temperature & 0x8000) {
		temperature = ~temperature + 1;
        data = 0.0 - (temperature / 16.0);
        return data;
	}
    data = temperature / 16.0;

    return data ;
}
uint8_t ds18b20_start_convert()
{
	uint8_t buff[2];
	_OW_Reset();
    buff[0]=0xcc;
    buff[1]=0x44;
    buff[0]=_OW_SwapByte(buff[0]);
    buff[1]=_OW_SwapByte(buff[1]);
    return 0;
}

void ds18b20_get_temp(uint8_t dev_id)
{

	uint8_t buff[12];
	_OW_Reset();
    buff[0]=0x55;
    memcpy(&buff[1], ds18_sensors[dev_id].rom_code, 8);
	buff[9]=0xbe;
    buff[10]=0xff;
	buff[11]=0xff;
	for(int i=0;i<12;i++)
	  buff[i]=_OW_SwapByte(buff[i]);
	ds18_sensors[dev_id].temp= ds18b20_tconvert(buff[10], buff[11]);

}

void ds18b20_init()
{
	owdevices=0;


	    OW_Search(ds18_sensors);

};


