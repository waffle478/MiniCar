/*
 * MPU_Handler.c
 *
 *  Created on: Jan 9, 2024
 *      Author: Csabi
 */

#include "MPU_Handler.h"

MPU_control MPU_data;

void MPU_Init(void)
{
	MPU_resetMessage();

	/* Setup SPI communication */
	MPU_data.UserControl.UserControlRegByte = 0;
	MPU_data.UserControl.I2C_IF_DIS = 1;

	MPU_data.Message.CircularDataBuffer[0] = WRITE | USER_CTRL;
	MPU_data.Message.CircularDataBuffer[1] = MPU_data.UserControl.UserControlRegByte;
	MPU_data.Message.MessageLenght = 2;

	SPI_AddMessageToQueue(&MPU_data.Message);

}

/* --------------------- [READ REGISTERS] --------------------- */

void MPU_getRegister(uint8_t regAddress)
{
	/* Set command to read the registers */
	regAddress |= READ;

	MPU_data.Message.CircularDataBuffer[0] = regAddress;
	MPU_data.Message.MessageLenght = 2;

	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_getRegisterMultipleData(uint8_t regAddress, uint8_t len)
{
	/* Set command to read the registers */
	regAddress |= READ;

	MPU_data.Message.CircularDataBuffer[0] = regAddress;
	MPU_data.Message.MessageLenght = 2;

	SPI_AddMessageToQueue(&MPU_data.Message);
}

/* --------------------- [SET REGISTERS] --------------------- */

void MPU_setRegister(uint8_t regAddress, uint8_t data)
{
	/* Read the data from the register so we do not write data we don't want to write */
	MPU_getRegister(regAddress);

	/* Setup Message so the earlier sent Message will be used to set the data */
	MPU_data.Message.RelatedTo.EarlierMessage = TRUE;
	MPU_data.Message.RelatedTo.Byte = 1;

	/* Assemble the modified data. */
	MPU_data.Message.CircularDataBuffer[1] = data;
	MPU_data.Message.CircularDataBuffer[0] = regAddress | WRITE;

	MPU_data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_setRegisterUnsafely(uint8_t regAddress, uint8_t data)
{
	/* Assemble the modified data. */
	MPU_data.Message.CircularDataBuffer[1] = data;
	MPU_data.Message.CircularDataBuffer[0] = regAddress | WRITE;

	MPU_data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	MPU_getRegister(regAddress);

	/* Assemble the modified data */
	for (int i = 0; i < len; ++i)
	{
		MPU_data.Message.CircularDataBuffer[i + 1] = MPU_data.Message.CircularDataBuffer[i];
		MPU_data.Message.CircularDataBuffer[i + 1] |= data[i];
	}
	/* First byte has to be set last to be set */
	MPU_data.Message.CircularDataBuffer[0] = regAddress | WRITE;

	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_setAddressData(uint8_t regAddress, uint8_t len)
{
	/* Read the data from the register so we do not write data we don't want to write */
	//getRegister(regAddress);

	/* Assemble the modified data */
	MPU_data.Message.CircularDataBuffer[0] = regAddress | WRITE;

	MPU_data.Message.MessageLenght = len;
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_receiveMessage(SPI_message message)
{
	memcpy(&MPU_data.ReceivedMessage, &message, sizeof(SPI_message));
}

void MPU_transferMessageToUart(UART_HandleTypeDef *huart)
{
	if (MPU_data.ReceivedMessage.MessageLenght > 0) {
		MPU_data.ReceivedMessage.CircularDataBuffer[MPU_data.ReceivedMessage.MessageLenght] = '\n';
		MPU_data.ReceivedMessage.MessageLenght++;

		HAL_UART_Transmit(huart, MPU_data.ReceivedMessage.CircularDataBuffer, MPU_data.ReceivedMessage.MessageLenght, DEFAULT_TIMEOUT);
		MPU_data.ReceivedMessage.MessageLenght = 0;
	}
}

void MPU_sendCommand(uint8_t command)
{
	MPU_data.Message.CircularDataBuffer[0] = command;

	MPU_data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_sendPayloadReadRequest()
{
	MPU_resetMessage();
	MPU_data.Message.Module.FunctionExecution = 1;
	MPU_data.Message.CircularDataBuffer[0] = READ | GYRO_XOUT_H;
	MPU_data.Message.MessageLenght = 2;
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_resetMessage()
{
	SPI_resetMessage(&MPU_data.Message);
	MPU_data.Message.Module.ModuleFunction = *MPU_receiveMessage;
	MPU_data.Message.Module.ModuleType = MODULE_TYPE_GYRO;
}
