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

#include "stm32l4xx_hal.h"
#include "Statuses.h"

#define SS_ENABLE_NRF01			GPIO_PIN_4
#define SS_REGISTER_NRF01		GPIOA

#ifdef GYRO
	#if GYRO == 1

#define SS_ENABLE_GYRO			2u
#define SS_REGISTER_GYRO		7u

#endif
#endif


#ifdef DISTANCE
	#if DISTANCE == 1

#define SS_ENABLE_DISTANCE		3u
#define SS_REGISTER_DISTANCE	8u

	#endif
#endif

typedef struct
{
	uint8_t 	EarlierMessage;
	uint8_t		Byte;
}RelatedMessage;

typedef struct
{
	uint8_t 		CircularDataBuffer[32];
	uint8_t			MessageLenght;
	RelatedMessage	RelatedTo;
	uint8_t			ModuleType;
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
