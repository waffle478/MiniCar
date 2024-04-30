/*
 * MPU_Handler.h
 *
 *  Created on: Jan 9, 2024
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_MPU_HANDLER_H_
#define APPLICATION_USER_INCLUDE_MPU_HANDLER_H_

#include <stdio.h>
#include "MPU9250_MemoryMap.h"
#include "SPI_Handler.h"

#define ReceivedMessage(INDEX) MPU_data.ReceivedMessage.CircularDataBuffer[INDEX]

#define MPU_FUNCTION_ACCELEROMETER_FS	1u
#define MPU_FUNCTION_GYROSCOPE_FS		2u
#define MPU_FUNCTION_ALL_DATA			3u

#define MPU_FS_ACCEL_2G		0u
#define MPU_FS_ACCEL_4G		1u
#define MPU_FS_ACCEL_8G		2u
#define MPU_FS_ACCEL_16G	3u

#define MPU_FS_GYRO_250		0u
#define MPU_FS_GYRO_500     1u
#define MPU_FS_GYRO_1000    2u
#define MPU_FS_GYRO_2000    3u

typedef struct{
	int16_t X_axis;
	int16_t Y_axis;
	int16_t Z_axis;
	int16_t FullScaleSelected;
	uint8_t DataReady;
}AxisRaw;

typedef union{
	uint8_t SIG_COND_RST : 1;
	uint8_t I2C_MST_RST : 1;
	uint8_t FIFO_RST : 1;
	uint8_t NOT_USED1 : 1;
	uint8_t I2C_IF_DIS  : 1;
	uint8_t I2C_MST_EN : 1;
	uint8_t FIFO_EN_reg : 1;
	uint8_t NOT_USED2 : 1;
	uint8_t UserControlRegByte;
}UserCtrlReg;

typedef struct{
	AxisRaw AccelerometerSensor;
	AxisRaw GyroSensor;
	uint16_t Temperature;
	UserCtrlReg UserControl;
	SPI_message Message;
	SPI_message	ReceivedMessage;
}MPU_control;

void MPU_Init(void);

void MPU_getRegister(uint8_t regAddress);

void MPU_setRegister(uint8_t regAddress, uint8_t data, uint8_t mask);
void MPU_setRegisterUnsafely(uint8_t regAddress, uint8_t data);

void MPU_sendCommand(uint8_t command);
void MPU_sendPayloadReadRequest(void);

void MPU_receiveMessage(SPI_message message);
void MPU_processFullScale_Accelerometer(uint8_t data);
void MPU_processFullScale_Gyroscope(uint8_t data);

void MPU_transferMessageToUart(UART_HandleTypeDef *huart);

void MPU_readSensors(void);

void MPU_resetMessage();

AxisRaw MPU_getAccelerometer(void);
AxisRaw MPU_getGyroscope(void);

uint8_t MPU_getReadyAccelerometer(void);
uint8_t MPU_getReadyGyroscope(void);

#endif /* APPLICATION_USER_INCLUDE_MPU_HANDLER_H_ */
