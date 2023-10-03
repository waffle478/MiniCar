/*
 * nRF24l01_Functions.h
 *
 *  Created on: Jul 23, 2023
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_
#define APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_

#define DEFAULT_TIMEOUT 30u

#define NSS_REGISTER 	GPIOA
#define NSS_PIN	 		GPIO_PIN_4

enum ResetBuffer{
	RESET_TRANSMIT_BUFFER,
	RESET_RECEIVE_BUFFER,
	RESET_ALL_BUFFER
};

typedef enum nRF24L01_status{
	SEND_PAYLOAD_LENGTH_READ_REQUEST,
	RECEIVE_MESSAGE_LENGTH,
	SEND_PAYLOAD_READ_REQUEST,
	RECEIVE_PAYLOAD,
}nRF24L01_status;

#include <nRF24l01_MemoryMap.h>
#include "stm32l4xx_hal.h"

typedef struct{
	uint8_t 	Status;
	uint8_t		State;
	uint8_t 	CircularDataBuffer[32];
}RF_Module;

void Setup(void);

void resetDataBuffer();

void getRegister(uint8_t regAddress);
void getRegisterMultipleData(uint8_t regAddress, uint8_t len);

void setRegister(uint8_t regAddress, uint8_t data);
void setRegisterUnsafely(uint8_t regAddress, uint8_t data);
void setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len);

void setAddressData(uint8_t regAddress, uint8_t len);

void sendPayloadReadRequest(void);

extern SPI_HandleTypeDef hspi1;
extern RF_Module RF_Data;

#endif /* APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_ */
