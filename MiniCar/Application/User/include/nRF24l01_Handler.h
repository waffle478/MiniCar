/*
 * nRF24l01_Functions.h
 *
 *  Created on: Jul 23, 2023
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_
#define APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_

#define DEFAULT_TIMEOUT 30u

enum ResetBuffer{
	RESET_TRANSMIT_BUFFER,
	RESET_RECEIVE_BUFFER,
	RESET_ALL_BUFFER
};

#include <nRF24l01_MemoryMap.h>
#include "SPI_Handler.h"

typedef struct{
	uint8_t 	Status;
	SPI_message Message;
	SPI_message	StagedMessage;
}RF_Module;

void RF_Setup(void);

void RF_getRegister(uint8_t regAddress);
void RF_getRegisterMultipleData(uint8_t regAddress, uint8_t len);

void RF_setRegister(uint8_t regAddress, uint8_t data);
void RF_setRegisterUnsafely(uint8_t regAddress, uint8_t data);
void RF_setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len);

void RF_setAddressData(uint8_t regAddress, uint8_t len);

void RF_sendCommand(uint8_t command);

void RF_sendPayloadReadRequest(void);

void RF_resetMessage();

extern SPI_HandleTypeDef hspi1;
extern RF_Module RF_Data;

#endif /* APPLICATION_USER_INCLUDE_NRF24L01_FUNCTIONS_H_ */
