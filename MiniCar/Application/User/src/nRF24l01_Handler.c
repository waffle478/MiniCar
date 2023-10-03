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
	resetDataBuffer();
	getRegister(CONFIG);

	sendCommand(FLUSH_RX);

	setRegisterUnsafely(CONFIG, 0x00);
	setRegisterUnsafely(STATUS, 0x70);

	RF_Data.Status = 0;
	resetDataBuffer();
	setRegisterUnsafely(EN_RXADDR, 0x02);


	setRegister(RX_PW_P0, 0x3f);

	setRegister(EN_AA, 0x3f);

	setRegisterUnsafely(RF_SETUP, 0x06);
	setRegister(RF_CH, 0x4c);

	setRegisterUnsafely(SETUP_AW, 0x03);
	setRegisterUnsafely(SETUP_RETR, 0x4f);
	setRegisterUnsafely(RX_PW_P0, 0x00);
	setRegisterUnsafely(RX_PW_P1, 0x01);

	resetDataBuffer();

	RF_Data.CircularDataBuffer[1] = '2';
	RF_Data.CircularDataBuffer[2] = 'N';
	RF_Data.CircularDataBuffer[3] = 'o';
	RF_Data.CircularDataBuffer[4] = 'd';
	RF_Data.CircularDataBuffer[5] = 'e';

	setAddressData(RX_ADDR_P0, 6);
	resetDataBuffer();

	RF_Data.CircularDataBuffer[1] = '1';
	RF_Data.CircularDataBuffer[2] = 'N';
	RF_Data.CircularDataBuffer[3] = 'o';
	RF_Data.CircularDataBuffer[4] = 'd';
	RF_Data.CircularDataBuffer[5] = 'e';

	setAddressData(RX_ADDR_P1, 6);
	resetDataBuffer();

	RF_Data.CircularDataBuffer[1] = '2';
	RF_Data.CircularDataBuffer[2] = 'N';
	RF_Data.CircularDataBuffer[3] = 'o';
	RF_Data.CircularDataBuffer[4] = 'd';
	RF_Data.CircularDataBuffer[5] = 'e';

	setAddressData(TX_ADDR, 6);
	resetDataBuffer();

	/* Start up the module */
	setRegister(CONFIG, (0x0f));

	getRegister(RX_PW_P0);
	resetDataBuffer();
}

void resetDataBuffer()
{
	/* TODO: Use other way to clear an array. */
	for (int i = 0; i < 30; ++i) {
		RF_Data.CircularDataBuffer[i] = 0xFF;
	}
}

void getRegister(uint8_t regAddress)
{
	uint8_t ret = 0;
	/* Set command to read the registers */
	regAddress |= R_REGISTER;

	RF_Data.CircularDataBuffer[0] = regAddress;

	/* Enable slave */
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	/* Send command to read the given register address and read the status at the same time */
	ret = HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, 2);
}

/*void getRegisterMultipleData(uint8_t regAddress, uint8_t len)
{
	/* Set command to read the registers */
	/*regAddress |= R_REGISTER;

	/* Enable slave */
	/*HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	/* Send command to read the given register address and read the status at the same time */
	/*HAL_SPI_TransmitReceive(&hspi1, &regAddress, &RF_Data.Status, 1, DEFAULT_TIMEOUT);
	/* Read the register data */
	//HAL_SPI_Receive(&hspi1, RF_Data.CircularDataBuffer, 32, DEFAULT_TIMEOUT);

	/* Disable slave */
	/*HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}*/

void setRegister(uint8_t regAddress, uint8_t data)
{
	/* Read the data from the register so we do not write data we don't want to write */
	getRegister(regAddress);

	/* Assemble the modified data. */
	RF_Data.CircularDataBuffer[1] |= data;
	RF_Data.CircularDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, 2);
}

void setRegisterUnsafely(uint8_t regAddress, uint8_t data)
{
	/* Assemble the modified data. */
	RF_Data.CircularDataBuffer[1] = data;
	RF_Data.CircularDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, 2);
}

/*void setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	/*getRegister(regAddress);

	/* Assemble the modified data */
	/*for (int i = 0; i < len; ++i)
	{
		RF_Data.CircularDataBuffer[i + 1] = RF_Data.CircularDataBuffer[i];
		RF_Data.CircularDataBuffer[i + 1] |= data[i];
	}
	/* First byte has to be set last to be set */
	/*RF_Data.CircularDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	/*HAL_SPI_Transmit_IT(&hspi1, RF_Data.CircularDataBuffer, len);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}*/

void setAddressData(uint8_t regAddress, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	//getRegister(regAddress);

	/* Assemble the modified data */
	RF_Data.CircularDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, len);
}

void sendCommand(uint8_t command)
{
	RF_Data.CircularDataBuffer[0] = command;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, 2);
}

void sendPayloadReadRequest(void)
{
	//RF_Data.CircularDataBuffer[0] = R_RX_PAYLOAD;

	//HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	//HAL_SPI_Receive_IT(&hspi1, RF_Data.CircularDataBuffer, 2);

	resetDataBuffer();
	getRegister(0xFF);
	sendCommand(R_RX_PAYLOAD);
}
