/*
 * nRF24l01_Functions.c
 *
 *  Created on: Jul 23, 2023
 *      Author: Csabi
 */

#include <nRF24l01_Handler.h>

RF_Module RF_Data;

void Setup(void)
{
	RF_Data.Status = 0;
	resetDataBuffers(RESET_ALL_BUFFER);
	setRegister(EN_RXADDR, 0x01);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
	setRegister(RX_PW_P0, 0x20);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
	setRegister(CONFIG, (0x3));
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);

	RF_Data.transmitDataBuffer[1] = '2';
	RF_Data.transmitDataBuffer[2] = 'N';
	RF_Data.transmitDataBuffer[3] = 'o';
	RF_Data.transmitDataBuffer[4] = 'd';
	RF_Data.transmitDataBuffer[5] = 'e';

	setAddressData(RX_ADDR_P0, 6);
	resetDataBuffers(RESET_ALL_BUFFER);
	getRegister(RX_PW_P0);
	//getRegister(CONFIG);

}

void resetDataBuffers(uint8_t bufferType)
{
	for (int i = 0; i < 30; ++i) {
		switch (bufferType) {
			case RESET_TRANSMIT_BUFFER:
				RF_Data.transmitDataBuffer[i] = 0;
				break;
			case RESET_RECEIVE_BUFFER:
				RF_Data.receiveDataBuffer[i] = 0;
				break;
			case RESET_ALL_BUFFER:
				RF_Data.transmitDataBuffer[i] = 0;
				RF_Data.receiveDataBuffer[i] = 0;
				break;
			default:
				/* DO NOTHING */
				break;
		}
	}
}

void getRegister(uint8_t regAddress)
{
	uint8_t ret = 0;
	/* Set command to read the registers */
	regAddress |= R_REGISTER;

	/* Enable slave */
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	/* Send command to read the given register address and read the status at the same time */
	ret = HAL_SPI_Transmit_IT(&hspi1, &regAddress, 1);

	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Read the register data */
	ret = HAL_SPI_Receive_IT(&hspi1, &RF_Data.receiveDataBuffer[0], 1);
}

void getRegisterMultipleData(uint8_t regAddress, uint8_t len)
{
	/* Set command to read the registers */
	regAddress |= R_REGISTER;

	/* Enable slave */
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	/* Send command to read the given register address and read the status at the same time */
	HAL_SPI_TransmitReceive(&hspi1, &regAddress, &RF_Data.Status, 1, DEFAULT_TIMEOUT);
	/* Read the register data */
	HAL_SPI_Receive(&hspi1, RF_Data.receiveDataBuffer, 32, DEFAULT_TIMEOUT);

	/* Disable slave */
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}

void setRegister(uint8_t regAddress, uint8_t data)
{
	/* Read the data from the register so we do not write data we don't want to write */
	getRegister(regAddress);

	/* Assemble the modified data */
	RF_Data.transmitDataBuffer[0] = regAddress | W_REGISTER;
	RF_Data.transmitDataBuffer[1] = RF_Data.receiveDataBuffer[0];
	RF_Data.transmitDataBuffer[1] |= data;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Transmit_IT(&hspi1, RF_Data.transmitDataBuffer, 2);
}

void setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	getRegister(regAddress);

	/* Assemble the modified data */
	RF_Data.transmitDataBuffer[0] = regAddress | W_REGISTER;

	for (int i = 0; i < len; ++i)
	{
		RF_Data.transmitDataBuffer[i + 1] = RF_Data.receiveDataBuffer[i];
		RF_Data.transmitDataBuffer[i + 1] |= data[i];
	}

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Transmit_IT(&hspi1, RF_Data.transmitDataBuffer, len);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}

void setAddressData(uint8_t regAddress, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	//getRegister(regAddress);

	/* Assemble the modified data */
	RF_Data.transmitDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Transmit_IT(&hspi1, RF_Data.transmitDataBuffer, len);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}

void sendPayloadReadRequest(void){
	resetDataBuffers(RESET_TRANSMIT_BUFFER);

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	HAL_SPI_Transmit_IT(&hspi1, (uint8_t*)R_RX_PAYLOAD, 1);
	HAL_SPI_Receive_IT(&hspi1, RF_Data.receiveDataBuffer, 32);
}
