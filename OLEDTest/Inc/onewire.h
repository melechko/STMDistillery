#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "stm32f1xx_hal.h"

/*#define OW_SEND_RESET	1
#define OW_NO_RESET		2

#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	3

#define OW_NO_READ		0xff
#define OW_READ_SLOT	0xff
uint8_t ow_state;
#define OW_STATE_NULL 0
#define OW_STATE_RESET1 1
#define OW_STATE_RESET2 2
#define OW_STATE_SEND  4
#define OW_STATE_COMPLEATE  8
#include "stm32f1xx_hal.h"
#define OW_STATE_SEARCH  16
#define OW_STATE_ERR 128
uint8_t LastDiscrepancy;
uint8_t LastDeviceFlag;
uint8_t DeviceID;*/

typedef struct
{
    uint8_t rom_code[8];
    uint8_t id;
    uint32_t itemp;
    float temp;
    uint8_t bError;
}owdevice_t;


//void OW_Search1(void);
extern uint8_t owdevices;
uint8_t OW_Search(owdevice_t *owdevices_);
void _OW_Reset(void);
uint8_t _OW_SwapByte(uint8_t data);


#endif /* ONEWIRE_H_ */



