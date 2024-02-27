/*
 * nRF24l01_Functions.c
 *
 *  Created on: Jul 23, 2023
 *      Author: Csabi
 */

#include <nRF24l01_Handler.h>

RF_Module RF_Data;

void RF_Setup(void)
{
	RF_Data.Message.ModuleType = MODULE_TYPE_NRF01;

	RF_resetMessage();
	RF_getRegister(CONFIG);

	RF_sendCommand(FLUSH_RX);

	RF_setRegisterUnsafely(CONFIG, 0x00);
	RF_setRegisterUnsafely(STATUS, 0x70);

	RF_Data.Status = 0;
	RF_resetMessage();
	RF_setRegisterUnsafely(EN_RXADDR, 0x02);


	RF_setRegister(RX_PW_P0, 0x3f);

	RF_setRegister(EN_AA, 0x3f);

	RF_setRegisterUnsafely(RF_SETUP, 0x06);
	RF_setRegister(RF_CH, 0x4c);

	RF_setRegisterUnsafely(SETUP_AW, 0x03);
	RF_setRegisterUnsafely(SETUP_RETR, 0x4f);
	RF_setRegisterUnsafely(RX_PW_P0, 0x00);
	RF_setRegisterUnsafely(RX_PW_P1, 0x01);

	RF_resetMessage();

	RF_Data.Message.CircularDataBuffer[1] = '2';
	RF_Data.Message.CircularDataBuffer[2] = 'N';
	RF_Data.Message.CircularDataBuffer[3] = 'o';
	RF_Data.Message.CircularDataBuffer[4] = 'd';
	RF_Data.Message.CircularDataBuffer[5] = 'e';

	RF_setAddressData(RX_ADDR_P0, 6);
	RF_resetMessage();

	RF_Data.Message.CircularDataBuffer[1] = '1';
	RF_Data.Message.CircularDataBuffer[2] = 'N';
	RF_Data.Message.CircularDataBuffer[3] = 'o';
	RF_Data.Message.CircularDataBuffer[4] = 'd';
	RF_Data.Message.CircularDataBuffer[5] = 'e';

	RF_setAddressData(RX_ADDR_P1, 6);
	RF_resetMessage();

	RF_Data.Message.CircularDataBuffer[1] = '2';
	RF_Data.Message.CircularDataBuffer[2] = 'N';
	RF_Data.Message.CircularDataBuffer[3] = 'o';
	RF_Data.Message.CircularDataBuffer[4] = 'd';
	RF_Data.Message.CircularDataBuffer[5] = 'e';

	RF_setAddressData(TX_ADDR, 6);
	RF_resetMessage();

	/* Start up the module */
	RF_setRegister(CONFIG, (0x0f));

	RF_getRegister(RX_PW_P0);
	RF_resetMessage();
}

void RF_getRegister(uint8_t regAddress)
{
	/* Set command to read the registers */
	regAddress |= R_REGISTER;

	RF_Data.Message.CircularDataBuffer[0] = regAddress;
	RF_Data.Message.MessageLenght = 2;

	SPI_AddMessageToQueue(&RF_Data.Message);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomment"
/*void getRegisterMultipleData(uint8_t regAddress, uint8_t len)
{
	/* Set command to read the registers */
	/*regAddress |= R_REGISTER;

	/* Enable slave */
	/*HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);

	/* Send command to read the given register address and read the status at the same time */
	/*HAL_SPI_TransmitReceive(&hspi1, &regAddress, &RF_Data.Status, 1, DEFAULT_TIMEOUT);*/
	/* Read the register data */
	//HAL_SPI_Receive(&hspi1, RF_Data.Message.CircularDataBuffer, 32, DEFAULT_TIMEOUT);*/

	/* Disable slave */
	/*HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}*/
#pragma GCC diagnostic pop

void RF_setRegister(uint8_t regAddress, uint8_t data)
{
	/* Read the data from the register so we do not write data we don't want to write */
	RF_getRegister(regAddress);

	/* Setup message so the earlier sent message will be used to set the data */
	RF_Data.Message.RelatedTo.EarlierMessage = TRUE;
	RF_Data.Message.RelatedTo.Byte = 1;

	/* Assemble the modified data. */
	RF_Data.Message.CircularDataBuffer[1] = data;
	RF_Data.Message.CircularDataBuffer[0] = regAddress | W_REGISTER;

	RF_Data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&RF_Data.Message);
}

void RF_setRegisterUnsafely(uint8_t regAddress, uint8_t data)
{
	/* Assemble the modified data. */
	RF_Data.Message.CircularDataBuffer[1] = data;
	RF_Data.Message.CircularDataBuffer[0] = regAddress | W_REGISTER;

	RF_Data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&RF_Data.Message);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomment"
/*void setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	/*getRegister(regAddress);

	/* Assemble the modified data */
	/*for (int i = 0; i < len; ++i)
	{
		RF_Data.Message.CircularDataBuffer[i + 1] = RF_Data.Message.CircularDataBuffer[i];
		RF_Data.Message.CircularDataBuffer[i + 1] |= data[i];
	}
	/* First byte has to be set last to be set */
	/*RF_Data.Message.CircularDataBuffer[0] = regAddress | W_REGISTER;

	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, RESET);
	/* Send out register data */
	/*HAL_SPI_Transmit_IT(&hspi1, RF_Data.Message.CircularDataBuffer, len);
	HAL_GPIO_WritePin(NSS_REGISTER, NSS_PIN, SET);
}*/
#pragma GCC diagnostic pop

void RF_setAddressData(uint8_t regAddress, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	//getRegister(regAddress);

	/* Assemble the modified data */
	RF_Data.Message.CircularDataBuffer[0] = regAddress | W_REGISTER;

	RF_Data.Message.MessageLenght = len;
	SPI_AddMessageToQueue(&RF_Data.Message);
}

void RF_sendCommand(uint8_t command)
{
	RF_Data.Message.CircularDataBuffer[0] = command;

	RF_Data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&RF_Data.Message);
}

void RF_sendPayloadReadRequest(void)
{
	RF_resetMessage();
	RF_sendCommand(R_RX_PAYLOAD);
}

void RF_resetMessage()
{
	SPI_resetMessage(&RF_Data.Message);
	RF_Data.Message.ModuleType = MODULE_TYPE_NRF01;
}
