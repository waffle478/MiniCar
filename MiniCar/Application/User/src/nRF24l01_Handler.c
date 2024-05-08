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
	RF_Data.Message.Module.ModuleType = MODULE_TYPE_NRF01;

	RF_resetMessage();
	RF_getRegister(CONFIG);

	RF_sendCommand(FLUSH_RX);

	/* Stop the RF MCU */
	RF_setRegisterUnsafely(CONFIG, 0x00);
	/* Clear status register */
	RF_setRegisterUnsafely(STATUS, 0x70);

	RF_Data.Status = 0;
	RF_resetMessage();
	/* Enable receive data pipe 1 */
	RF_setRegisterUnsafely(EN_RXADDR, 0x02); // should set pipe 0 for transmit?

	/* Enable AutoAcknowledge for all pipes (for some reason)*/
	RF_setRegister(EN_AA, 0x3f);

	/* Set The output power to 0dB and data transmit speed to 1Mbps */
	RF_setRegisterUnsafely(RF_SETUP, 0x06);
	/* Setting channel for communication */
	RF_getRegister(RF_CH);
	RF_setRegisterUnsafely(RF_CH, 0x4c);
	RF_getRegister(RF_CH);

	/* Address width to 5 bytes */
	RF_setRegisterUnsafely(SETUP_AW, 0x03);
	/* Automatic retransmission - 1250 us max wait and max 15 retransmittion */
	RF_setRegisterUnsafely(SETUP_RETR, 0x4f);

	/* Set number of bytes to receive */
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

	/* Start up the module and set receive mode*/
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
	RF_resetMessage();
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

void RF_receiveMessage(SPI_message message)
{
	memcpy(&RF_Data.ReceivedMessage, &message, sizeof(SPI_message));
}

void RF_transferMessageToUart(UART_HandleTypeDef *huart)
{
	if (RF_Data.ReceivedMessage.MessageLenght > 0 && RF_Data.ReceivedMessage.CircularDataBuffer[0] == 0x42) {
		RF_Data.ReceivedMessage.CircularDataBuffer[RF_Data.ReceivedMessage.MessageLenght] = '\r';
		RF_Data.ReceivedMessage.MessageLenght++;
		RF_Data.ReceivedMessage.CircularDataBuffer[RF_Data.ReceivedMessage.MessageLenght] = '\n';

		HAL_UART_Transmit(huart, &RF_Data.ReceivedMessage.CircularDataBuffer[1], RF_Data.ReceivedMessage.MessageLenght, DEFAULT_TIMEOUT + 100);
		RF_Data.ReceivedMessage.MessageLenght = 0;
	}
	else
	{
		//HAL_UART_Transmit(huart, ".", 1, DEFAULT_TIMEOUT);
	}
}

void RF_sendPayloadReadRequest(void)
{
	RF_resetMessage();
	RF_sendCommand(R_RX_PAYLOAD);
}

void RF_resetMessage()
{
	SPI_resetMessage(&RF_Data.Message);
	RF_Data.Message.Module.ModuleFunction = *RF_receiveMessage;
	RF_Data.Message.Module.FunctionExecution = 1;
	RF_Data.Message.Module.ModuleType = MODULE_TYPE_NRF01;
}
