#include "ds18b20.h"
#include <string.h>
#include "TM1638.h"
const uint8_t cSensor1[8]= {0x28,0xa3,0x0f,0x78,0x0e,0x00,0x00,0x34};
const uint8_t cSensor3[8]= {0x28,0x42,0xb7,0xe0,0x0c,0x00,0x00,0xe9};
const uint8_t cSensor2[8]= {0x28,0x03,0xc0,0xe0,0x0c,0x00,0x00,0x97};
uint8_t dev_index[3]={255,255,255};
owdevice_t ds18_sensors[MAX_SENSORS];
      //rom code
uint8_t owdevices = 0;
//uint32_t ow_tickstart=0;
//static uint8_t curr_device=0xff;//devices index

void ds18b20_tconvert(uint8_t LSB, uint8_t MSB,owdevice_t *owdevices_)
{
    //float data;

    uint16_t temperature;

    temperature = LSB | (MSB << 8);
    if(temperature==0xffff){
    	if(owdevices_->bError<5)
    	  owdevices_->bError++;
    	if(owdevices_->bError>=5)
    	  owdevices_->itemp=0x79505000;
    	return;
    }
    owdevices_->bError=0;
    owdevices_->itemp=0;
	if (temperature & 0x8000) {
		temperature = ~temperature + 1;
		owdevices_->itemp=digitToSegment[0x10];
		owdevices_->temp = 0.0 - (temperature / 16.0);
       // return data;
	}
	else{
	owdevices_->temp = temperature / 16.0;
	  if(temperature>1599){
		owdevices_->itemp=digitToSegment[0x1];
		temperature-=1600;
	  }
	}

	owdevices_->itemp=(owdevices_->itemp<<8)+digitToSegment[temperature / 160];
	temperature%=160;
	owdevices_->itemp=(owdevices_->itemp<<8)+(digitToSegment[temperature >>4]|0x80);
	temperature&=0xf;
	owdevices_->itemp=(owdevices_->itemp<<8)+digitToSegment[(temperature* 5)>>3];

    //return data ;
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
	ds18b20_tconvert(buff[10], buff[11],&ds18_sensors[dev_id]);

}

void ds18b20_init()
{
	owdevices=0;
    dev_index[0]=255;
    dev_index[1]=255;
    dev_index[2]=255;

	OW_Search(ds18_sensors);
	for (int i = 0; i < owdevices; i++) {
  	  if(memcmp(ds18_sensors[i].rom_code,cSensor1,8)==0)
	    dev_index[0]=i;
	  else
	  if(memcmp(ds18_sensors[i].rom_code,cSensor2,8)==0)
	    dev_index[1]=i;
	  else
	  if(memcmp(ds18_sensors[i].rom_code,cSensor3,8)==0)
	    dev_index[2]=i;
    }

};


