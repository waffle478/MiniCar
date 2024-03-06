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

	MPU_setRegisterUnsafely(USER_CTRL, MPU_data.UserControl.UserControlRegByte);

	MPU_resetMessage();
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

void MPU_receiveMessage(SPI_message message)
{
	/* TODO: test this function */
	memcpy(&MPU_data.ReceivedMessage, &message, sizeof(SPI_message));
	/* Copy the incoming data to the sensor memory location. */
	memcpy(MPU_data.Sensors.Memory, &message.CircularDataBuffer[1], message.MessageLenght);
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

void MPU_readSensors()
{
	MPU_resetMessage();

	/* Send commands to get accel, temp, and gyroscope data */
	uint8_t command = ACCEL_XOUT_H;
	for (; command < GYRO_ZOUT_L; command++) {
		// Maybe use sendCommand function instead? Function needs change.
		MPU_data.Message.CircularDataBuffer[command - ACCEL_XOUT_H] = 0;
	}
	MPU_data.Message.Module.FunctionExecution = 1;
	/* After the FOR cycle this will be the length of the message. */
	MPU_data.Message.MessageLenght = command - ACCEL_XOUT_H + 1; // We need plus 1 length to get the last data.
	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_resetMessage()
{
	SPI_resetMessage(&MPU_data.Message);
	MPU_data.Message.Module.ModuleFunction = *MPU_receiveMessage;
	MPU_data.Message.Module.ModuleType = MODULE_TYPE_GYRO;
}
