#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "stm32f1xx_hal.h"

#define OW_SEND_RESET	1
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
uint8_t LastDiscrepancy;       /*!< Search private */
uint8_t LastDeviceFlag;        /*!< Search private */
uint8_t DeviceID;

typedef struct
{
    uint8_t rom_code[8];
    uint8_t id;
    float temp;
}owdevice_t;
extern void ds18b20Compleate(uint8_t *data);
uint8_t OW_Init(void);

void OW_Search1(void);
uint8_t OW_Search2(owdevice_t *owdevices);


#endif /* ONEWIRE_H_ */



