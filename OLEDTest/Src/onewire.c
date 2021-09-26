#include "onewire.h"



extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;


#define OW_0	        0x00
#define OW_1	        0xff
#define OW_R_1	        0xff

#define OW_CMD_SEARCH   0xf0


/*uint8_t ow_buf[8];


#define OW_0	        0x00
#define OW_1	        0xff
#define OW_R_1	        0xff

#define OW_CMD_SEARCH   0xf0
#define OW_CMD_MATCH    0x55


static uint8_t ROM_NO[8];

static uint8_t data[16];
static uint8_t data_len;
static uint8_t current_pos;*/


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

static void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if (ow_byte & 0x01) {
			*ow_bits = OW_1;
		} else {
			*ow_bits = OW_0;
		}
		ow_bits++;
		ow_byte = ow_byte >> 1;
	}
}

static uint8_t OW_toByte(uint8_t *ow_bits) {
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++) {
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_1) {
			ow_byte |= 0x80;
		}

		ow_bits++;
	}
	return ow_byte;
}





uint8_t OW_SendBits(uint8_t data) {
	uint8_t d=data?0xff:0;
	HAL_UART_Transmit(&huart2,&d,1,100);
	HAL_UART_Receive(&huart2,&d,1,100);
    return d;


}

uint8_t OW_ReadBit(void)
{


    if(OW_SendBits(1) == 0xff) {
        return 1;
    }
    else {
        return 0;
    }
}


uint8_t OW_Search(owdevice_t *owdevices_) {
	uint8_t LastDeviceFlag = 0;
	uint8_t LastDiscrepancy =0;
	uint8_t ROM_NO[8];
	while (1) {
		uint8_t search_result = 0;
		_OW_Reset();
		_OW_SwapByte(OW_CMD_SEARCH);
		{
			uint8_t id_bit_number = 1;
			uint8_t last_zero = 0, rom_byte_number = 0;
			uint8_t id_bit, cmp_id_bit;
			uint8_t rom_byte_mask = 1, search_direction;
			if (!LastDeviceFlag) {

				while (hdma_usart2_rx.State == HAL_DMA_STATE_BUSY)
					;
				// loop to do the search
				do {
					// read a bit and its complement
					id_bit = OW_ReadBit();
					cmp_id_bit = OW_ReadBit();

					// check for no devices on 1-wire
					if ((id_bit == 1) && (cmp_id_bit == 1)) {
						break;
					} else {
						// all devices coupled have 0 or 1
						if (id_bit != cmp_id_bit) {
							search_direction = id_bit; // bit write value for search
						} else {
							// if this discrepancy if before the Last Discrepancy
							// on a previous next then pick the same as last time
							if (id_bit_number < LastDiscrepancy) {
								search_direction = ((ROM_NO[rom_byte_number]
										& rom_byte_mask) > 0);
							} else {
								// if equal to last pick 1, if not then pick 0
								search_direction = (id_bit_number
										== LastDiscrepancy);
							}

							// if 0 was picked then record its position in LastZero
							if (search_direction == 0) {
								last_zero = id_bit_number;
							}
						}

						// set or clear the bit in the ROM byte rom_byte_number
						// with mask rom_byte_mask
						if (search_direction == 1) {
							ROM_NO[rom_byte_number] |= rom_byte_mask;
						} else {
							ROM_NO[rom_byte_number] &= ~rom_byte_mask;
						}

						// serial number search direction write bit
						//OW_toBits(search_direction, ow_buf);
						OW_SendBits(search_direction);

						// increment the byte counter id_bit_number
						// and shift the mask rom_byte_mask
						id_bit_number++;
						rom_byte_mask <<= 1;

						// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
						if (rom_byte_mask == 0) {
							//docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
							rom_byte_number++;
							rom_byte_mask = 1;
						}
					}
				} while (rom_byte_number < 8); // loop until through all ROM bytes 0-7

				// if the search was successful then
				if (!(id_bit_number < 65)) {
					// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
					LastDiscrepancy = last_zero;

					// check for last device
					if (LastDiscrepancy == 0) {
						LastDeviceFlag = 1;
					}

					search_result = 1;
					owdevices_[owdevices].id = owdevices;
					for (int i = 0; i < 8; i++)
						owdevices_[owdevices].rom_code[i] = ROM_NO[i];
					//owdevices++;
				}

				// if no device found then reset counters so next 'search' will be like a first
				if (!search_result) {
					LastDiscrepancy = 0;
					LastDeviceFlag = 0;
					search_result = 0;
					//DeviceID = 0;
				}
			}

		}
		if (search_result) {
			if (ds18b20_crc8(owdevices_[owdevices].rom_code, 7)
					== owdevices_[owdevices].rom_code[7]) {
				owdevices_[owdevices].bError=0;
				owdevices++;
			}
		} else
			break;
	}
	return owdevices;
}
;

void _OW_Reset(void){
	uint8_t ow_buf[1];
	huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_HalfDuplex_Init(&huart2);

	ow_buf[0]=0xf0;

	HAL_UART_Transmit_DMA(&huart2,ow_buf,1);
	HAL_UART_Receive_DMA(&huart2,ow_buf,1);
	while(hdma_usart2_rx.State==HAL_DMA_STATE_BUSY){
		HAL_Delay(1);
	}
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_HalfDuplex_Init(&huart2);
	//      ow_state&=~OW_STATE_RESET2;
	//      if(ow_buf[0]==0xf0){
};
uint8_t _OW_SwapByte(uint8_t data){
	uint8_t buff[8];
	OW_toBits(data,buff);
	for(int i=0;i<8;i++){
		HAL_UART_Transmit(&huart2,&buff[i],1,100);
		HAL_UART_Receive(&huart2,&buff[i],1,100);
	}
	data=OW_toByte(buff);
	return data;
};
