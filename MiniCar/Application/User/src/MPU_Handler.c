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

	/* TODO: Add a function so the sensor could be calibrated */
		/* Calibration means the offsets are set so the read values are zero */

	/* Set full scale values */
	MPU_resetMessage(&MPU_data.Message);
	MPU_setRegisterUnsafely(ACCEL_CONFIG, MPU_FS_ACCEL_4G<<3);
	MPU_resetMessage(&MPU_data.Message);
	MPU_setRegister(MPU_CONFIG, 1, 0xF8);
	MPU_resetMessage(&MPU_data.Message);
	MPU_setRegisterUnsafely(GYRO_CONFIG, MPU_FS_GYRO_1000<<3 | 3);

	MPU_resetMessage();
	/* Read back full scale values and process them */
	MPU_resetMessage(&MPU_data.Message);
	MPU_data.Message.Module.FunctionExecution = 1;
	MPU_data.Message.Module.FunctionType = MPU_FUNCTION_ACCELEROMETER_FS;
	MPU_getRegister(ACCEL_CONFIG);

	MPU_resetMessage(&MPU_data.Message);
	MPU_data.Message.Module.FunctionExecution = 1;
	MPU_data.Message.Module.FunctionType = MPU_FUNCTION_GYROSCOPE_FS;
	MPU_getRegister(GYRO_CONFIG);
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

void MPU_setRegister(uint8_t regAddress, uint8_t data, uint8_t mask)
{
	/* Read the data from the register so we do not write data we don't want to write */
	MPU_getRegister(regAddress);

	/* Setup Message so the earlier sent Message will be used to set the data */
	MPU_data.Message.RelatedTo.EarlierMessage = TRUE;
	MPU_data.Message.RelatedTo.Byte = 1;

	/* TODO: add a mask to the relatedTo struct */
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
	/* Copy the incoming data to the sensor memory location. */
	memcpy(&MPU_data.ReceivedMessage, &message, sizeof(SPI_message));

	switch (MPU_data.ReceivedMessage.Module.FunctionType) {
		case MPU_FUNCTION_ACCELEROMETER_FS:
			MPU_processFullScale_Accelerometer(MPU_data.ReceivedMessage.CircularDataBuffer[1]);
			break;
		case MPU_FUNCTION_GYROSCOPE_FS:
			MPU_processFullScale_Gyroscope(MPU_data.ReceivedMessage.CircularDataBuffer[1]);
			break;
		/* Processes all sensor data received from MPU */  /*TODO: Probably I could add single functions for single data reads */
		case MPU_FUNCTION_ALL_DATA:
			MPU_data.AccelerometerSensor.X_axis = (ReceivedMessage(1) << 8) | ReceivedMessage(2);
			MPU_data.AccelerometerSensor.Y_axis = (ReceivedMessage(3) << 8) | ReceivedMessage(4);
			MPU_data.AccelerometerSensor.Z_axis = (ReceivedMessage(5) << 8) | ReceivedMessage(6);
			MPU_data.AccelerometerSensor.DataReady = 1;

			MPU_data.Temperature = (ReceivedMessage(7) << 8) | ReceivedMessage(8);

			MPU_data.GyroSensor.Y_axis = (ReceivedMessage(9) << 8) | ReceivedMessage(10);
			MPU_data.GyroSensor.Z_axis = (ReceivedMessage(11) << 8) | ReceivedMessage(12);
			MPU_data.GyroSensor.X_axis = (ReceivedMessage(13) << 8) | ReceivedMessage(14);
			MPU_data.GyroSensor.DataReady = 1;
			break;
		default:
			break;
	}

}

/* TODO: create a processFullScale function that can handle both acc and gyro sensors */
void MPU_processFullScale_Accelerometer(uint8_t data)
{
	uint8_t fs_bin = (data & 0x18) >> 3;

	switch (fs_bin) {
		case MPU_FS_ACCEL_2G:
			MPU_data.AccelerometerSensor.FullScaleSelected = 2;
			break;
		case MPU_FS_ACCEL_4G:
			MPU_data.AccelerometerSensor.FullScaleSelected = 4;
			break;
		case MPU_FS_ACCEL_8G:
			MPU_data.AccelerometerSensor.FullScaleSelected = 8;
			break;
		case MPU_FS_ACCEL_16G:
			MPU_data.AccelerometerSensor.FullScaleSelected = 16;
			break;
		default:
			break;
	}
}

void MPU_processFullScale_Gyroscope(uint8_t data)
{
	uint8_t fs_bin = (data & 0x18) >> 3;

	switch (fs_bin) {
		case MPU_FS_GYRO_250:
			MPU_data.GyroSensor.FullScaleSelected = 250;
			break;
		case MPU_FS_GYRO_500:
			MPU_data.GyroSensor.FullScaleSelected = 500;
			break;
		case MPU_FS_GYRO_1000:
			MPU_data.GyroSensor.FullScaleSelected = 1000;
			break;
		case MPU_FS_GYRO_2000:
			MPU_data.GyroSensor.FullScaleSelected = 2000;
			break;
		default:
			break;
	}
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
	MPU_data.Message.Module.FunctionType = MPU_FUNCTION_ALL_DATA;

	MPU_data.Message.CircularDataBuffer[0] = READ | ACCEL_XOUT_H;
	MPU_data.Message.MessageLenght = 15;

	SPI_AddMessageToQueue(&MPU_data.Message);
}

void MPU_resetMessage()
{
	SPI_resetMessage(&MPU_data.Message);
	MPU_data.Message.Module.ModuleFunction = *MPU_receiveMessage;
	MPU_data.Message.Module.ModuleType = MODULE_TYPE_GYRO;
}



AxisRaw MPU_getAccelerometer(void)
{
	return MPU_data.AccelerometerSensor;
}

AxisRaw MPU_getGyroscope(void)
{
	return MPU_data.GyroSensor;
}

uint8_t MPU_getReadyAccelerometer(void)
{
	uint8_t retVal = MPU_data.AccelerometerSensor.DataReady;
	MPU_data.AccelerometerSensor.DataReady = 0;
	return retVal;
}
uint8_t MPU_getReadyGyroscope(void)
{
	uint8_t retVal = MPU_data.GyroSensor.DataReady;
	MPU_data.GyroSensor.DataReady = 0;
	return retVal;
}

