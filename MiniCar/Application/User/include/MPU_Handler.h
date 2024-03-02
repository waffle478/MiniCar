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

typedef struct{
	uint16_t X_axis;
	uint16_t Y_axis;
	uint16_t Z_axis;
}AxisData;

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
	AxisData GyroSensor;
	AxisData AccelerometerSensor;
	uint8_t Temperature;
	UserCtrlReg UserControl;
	SPI_message Message;
}MPU_control;

void MPU_Init(void);

void MPU_getRegister(uint8_t regAddress);
void MPU_getRegisterMultipleData(uint8_t regAddress, uint8_t len);

void MPU_setRegister(uint8_t regAddress, uint8_t data);
void MPU_setRegisterUnsafely(uint8_t regAddress, uint8_t data);
void MPU_setRegisterMultipleData(uint8_t regAddress, uint8_t* data, uint8_t len);

void MPU_setAddressData(uint8_t regAddress, uint8_t len);

void MPU_sendCommand(uint8_t command);

void MPU_sendPayloadReadRequest(void);

void MPU_resetMessage();

#endif /* APPLICATION_USER_INCLUDE_MPU_HANDLER_H_ */
