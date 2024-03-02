/*
 * SPI_Handler.h
 *
 *  Created on: 2023. nov. 11.
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_SPI_HANDLER_H_
#define APPLICATION_USER_INCLUDE_SPI_HANDLER_H_

#define MODULE_TYPE_NONE		0u
#define MODULE_TYPE_NRF01		1u
#define MODULE_TYPE_GYRO		2u
#define MODULE_TYPE_DISTANCE	3u

#define QUEUE_MAX_MESSAGES		30u

#define ENABLED		1
#define DISABLED	0

#define GYRO		ENABLED

#define MODULE_FUNCTION_POINTER(NAME) void(*NAME)(SPI_message)

#define CURRENT_QUEUE_ELEMENT	Data.Queue.Queue[0]
#define NEXT_QUEUE_ELEMENT		Data.Queue.Queue[1]

#include "main.h"
#include "stm32l4xx_hal.h"
#include "Statuses.h"


typedef struct
{
	uint8_t 	EarlierMessage;
	uint8_t		Byte;
}RelatedMessage;

typedef struct
{
	uint8_t		ModuleType;
	uint8_t		FunctionExecution;
	MODULE_FUNCTION_POINTER(ModuleFunction);
}ModuleData;

typedef struct
{
	uint8_t 		CircularDataBuffer[32];
	uint8_t			MessageLenght;
	RelatedMessage	RelatedTo;
	ModuleData		Module;
}SPI_message;

typedef struct{
	uint8_t 		Status;
	SPI_message		Queue[QUEUE_MAX_MESSAGES];
	uint8_t			QueueLength;
}SPI_queue;

typedef struct{
	SPI_queue	Queue;
	SPI_HandleTypeDef* HalSpiPort;
}SPI_Data;

uint8_t SPI_AddMessageToQueue(SPI_message *messageAddress);
uint8_t SPI_INIT(SPI_HandleTypeDef *);
void SPI_EmptyQueue();
void SPI_AdvanceQueue();
void SPI_Cycle();
void SPI_EnableSSPin(uint8_t Type);
void SPI_DisableSSPin(uint8_t Type);
void SPI_resetMessage(SPI_message *message);

#endif /* APPLICATION_USER_INCLUDE_SPI_HANDLER_H_ */
