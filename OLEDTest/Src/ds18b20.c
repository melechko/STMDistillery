#include "ds18b20.h"
#include "stm32f1xx_hal.h"
#include "onewire.h"


      //rom code
uint8_t owdevices = 0;
uint32_t ow_tickstart=0;
static uint8_t curr_device=0xff;//devices index

uint8_t ds18b20_crc8(uint8_t *addr, uint8_t len) {
	uint8_t crc = 0, inbyte, i, mix;

	while (len--) {
		inbyte = *addr++;
		for (i = 8; i; i--) {
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) {
				crc ^= 0x8C;
			}
			inbyte >>= 1;
		}
	}
	return crc;
}

uint8_t ds18b20_init(void)
{
    if(!OW_Init()) {
        return 0;
    }
    LastDiscrepancy = 0;   /* Reset the search */
    LastDeviceFlag = 0;
    DeviceID = 0;
    while(1){
      OW_Search1();
      if(OW_Search2(ds18_sensors)){
        if(ds18b20_crc8(ds18_sensors[owdevices].rom_code, 7) == ds18_sensors[owdevices].rom_code[7]) {
          owdevices++;
        }
      }
      else
    	break;
    }
    return owdevices;
}

uint8_t ds18b20_start_convert(void)
{
    uint8_t send_buf[2] = {0xcc, 0x44};
    OW_Send(OW_SEND_RESET, send_buf, sizeof(send_buf));

    return 0;
}

void ds18b20_get_temp(uint8_t dev_id)
{
  //  uint8_t fbuf[2];
    uint8_t send_buf[12];



    send_buf[0] = 0x55;
    memcpy(&send_buf[1], ds18_sensors[dev_id].rom_code, 8);
    send_buf[9] = 0xbe;
    send_buf[10] = 0xff;
    send_buf[11] = 0xff;

    OW_Send(OW_SEND_RESET, send_buf, sizeof(send_buf));//, fbuf, sizeof(fbuf), 10);

    //temp = ds18b20_tconvert(fbuf[0], fbuf[1]);

}

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
void ds18b20Compleate(uint8_t *data){
	if(curr_device>owdevices){
		//Команда конвертации
		//ow_state&=~OW_STATE_COMPLEATE;

	}
	else{
		if(ow_state==OW_STATE_COMPLEATE){
		  ds18_sensors[curr_device].temp= ds18b20_tconvert(data[10], data[11]);
		  ow_state=OW_STATE_NULL;
		  curr_device++;
		}

		//else
		//	GetDeviceData();
	}

};
void GetDeviceData(){
	if(curr_device==owdevices){
				curr_device=0xff;
				ow_tickstart = HAL_GetTick();
			    ds18b20_start_convert();
			    return;
			}
	if(curr_device>owdevices){
		if(HAL_GetTick()-ow_tickstart>1000){
		  if((ow_state==OW_STATE_COMPLEATE)||(ow_state==OW_STATE_NULL)){
			  ow_state&=~OW_STATE_COMPLEATE;
		      curr_device=0;
		  }
		}
	}
	else
	if(ow_state==OW_STATE_NULL)
	  ds18b20_get_temp(curr_device);
};
